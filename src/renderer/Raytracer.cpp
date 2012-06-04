#include "renderer/raytracer.h"
#include "renderer/Miro.h"
#include "geometry/Scene.h"
#include "renderer/Camera.h"
#include "renderer/Image.h"
#include "sysutils/Console.h"
#include <time.h>
#include <Windows.h>

#include "pthread/pthread.h"
#include "renderer/glut.h"
#include "renderer/RenderingStats.h"
#include "sysutils/Random.h"

#if RENDERING_STATS
extern RenderingStats* rendering_statistics;
#endif

#define THREADS 8 // Including the main thread

#define PIXEL_SAMPLES 4

Raytracer::Raytracer()
{

}

//static int num_ray_object_intersections = 0;

/*
Pointer to this class given to each thread in the MT setup
*/

class RaytracerScanlinePool
{
private: 
	int m_currentScanline;
	bool* m_scanlinesFinished;
	int m_numScanlinesFinished;
	pthread_mutex_t mutex_general;
public:
	Scene* scene;
	Camera* camera;
	Image* image;

	RaytracerScanlinePool(Scene* scene, Camera* camera, Image* image)
	{
		this->scene = scene;
		this->camera = camera;
		this->image = image;

		m_scanlinesFinished = new bool[image->height()];
		for(int i = 0; i < image->height(); i++)
		{
			m_scanlinesFinished[i] = false;
		}

		m_numScanlinesFinished = 0;
		m_currentScanline = -1;
		mutex_general = PTHREAD_MUTEX_INITIALIZER;
	}

	~RaytracerScanlinePool()
	{
		delete m_scanlinesFinished;
	}

	bool isScanlineFinished(int scanline)
	{
		bool v;
		lock();
		v = m_scanlinesFinished[scanline];
		unlock();
		return v;
	}

	void lock() // Lock on access to the job class
	{
		pthread_mutex_lock(&mutex_general);
	}

	void unlock()
	{
		pthread_mutex_unlock(&mutex_general);
	}

	int nextScanLine(int previous_scanline_now_completed)
	{
		int v;
		lock();
		v = m_currentScanline < image->height() -1 ? ++m_currentScanline : -1;

		if(previous_scanline_now_completed >= 0)
		{
			registerScanlineFinished(previous_scanline_now_completed);
		}

		unlock();
		return v;
	}

private:
	void registerScanlineFinished(int scanline)
	{
		m_numScanlinesFinished++;
		m_scanlinesFinished[scanline] = true;
	}

};

/*
Draw one line of the screen
*/

static void drawScanLineY(int scanline, RaytracerScanlinePool* job)
{
	Ray ray;
    HitInfo hitInfo;    

	for (int i = 0; i < job->image->width(); ++i)
	{
		Vector3 shadeResultAccum2 (0);

		//std::vector<Vector3> colors; std::vector<Ray> rays;
		//colors.clear();

		double x = 0, y = 0, z = 0;

		for(int sample = 0; sample < PIXEL_SAMPLES; sample++)
		{
			float dx = Random::uniformRand() - 0.5;
			float dy = Random::uniformRand() - 0.5;

#if PIXEL_SAMPLES == 1
			dx = 0;
			dy = 0;
#endif

			ray = job->camera->eyeRay(i+dx, scanline+dy, job->image->width(), job->image->height());

			if (job->scene->trace(hitInfo, ray))
			{
				Vector3 color = hitInfo.material->shade(ray, hitInfo, *job->scene, 0);
				/*colors.push_back(color);
				rays.push_back(ray);
				x += color.x;
				y += color.y;
				z += color.z;
				*/

				shadeResultAccum2 += color / float(PIXEL_SAMPLES);
			}
		}
		
		


		/*float lim = 169/255.0;

		if(shadeResultAccum2.x > lim && shadeResultAccum2.y > lim  && shadeResultAccum2.z > lim )
		{
			double x = 0, y = 0, z = 0;
			printf("All bright! Color: ");
			printf("%g %g %g\n------\n", shadeResultAccum2.x, shadeResultAccum2.y, shadeResultAccum2.z);

			for(int i =0; i < colors.size(); i++)
			{
				printf("Color: %g %g %g\n", colors.at(i).x, colors.at(i).y, colors.at(i).z);
				printf("Ray 0: %g %g %g\n", rays.at(i).origin().x, rays.at(i).origin().y, rays.at(i).origin().z);
				Vector3 retraced = hitInfo.material->shade(rays.at(i), hitInfo, *job->scene, 0);
				printf("Color Retraced: %g %g %g\n------\n", retraced.x, retraced.y, retraced.z);
				x += retraced.x;
				y += retraced.y;
				z += retraced.z;

			}
			printf("Avg: %g %g %g\n------\n", x/ double(PIXEL_SAMPLES), y/ double(PIXEL_SAMPLES), z/ double(PIXEL_SAMPLES));


			throw 100;
		}*/
		

		job->image->setPixel(i, scanline, shadeResultAccum2);
	}
}

/*
// Each thread gets next scan line from the pool
*/

static void drawNextScanlines(void* arg)
{
	RaytracerScanlinePool* job = (RaytracerScanlinePool*)arg;

	Ray ray;
	HitInfo hitInfo;
	Vector3 shadeResult;

	int scanline = -1;
	while((scanline = job->nextScanLine(scanline)) != -1)
	{
		drawScanLineY(scanline, job);
	}
}

/*
// Draw Scene Method
*/

void Raytracer::drawScene(Scene& scene, Camera& camera, Image* image)
{
	//num_ray_object_intersections = 0;
	
	LARGE_INTEGER frequency, tick_start, tick_end;
    float elapsedTime;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&tick_start);

	RaytracerScanlinePool job (&scene, &camera, image);

#if THREADS > 1

	pthread_t threads[THREADS-1];

	pthread_attr_t attr;
	pthread_attr_init(&attr);

	// Start all threads

	for(int thread = 0; thread < THREADS-1; thread++)
	{
		int returnCode = pthread_create(&threads[thread], &attr, (void*(*)(void*))&drawNextScanlines, (void *)&job);

		if(returnCode)
		{
			printf("Error; return code from pthread_create() is %d\n", returnCode);
			exit(3);
		}
	}

#endif

	// Main thread is also drawing, and flushing work to the screen.
	// It also prints a summary to stdout

	printf("---------------------------------------------------------------\n");

	if(g_camera->focalLength() > 0)
	{
		printf("Using depth-of-field with focallength %g and aperture %g.\n", g_camera->focalLength(), g_camera->aperture());
	}

	int scanline = -1, prev_scanline_drawn_to_screen = 0;
	while((scanline = job.nextScanLine(scanline)) != -1)
	{
		drawScanLineY(scanline, &job);

		// Check for new scanlines to draw to screen

		for(int i = prev_scanline_drawn_to_screen; i < image->height(); i++)
		{
			if(job.isScanlineFinished(i))
			{
				QueryPerformanceCounter(&tick_end);
				elapsedTime = (float) (tick_end.QuadPart - tick_start.QuadPart)  / frequency.QuadPart;
				printf("Rendering %d samples: %.2f%% at %.2f seconds. Guess %.2f seconds.\r", PIXEL_SAMPLES,
						(float)i/image->height()*100, elapsedTime, elapsedTime*float(image->height())/float(i));
				prev_scanline_drawn_to_screen++;
				image->drawScanline(i);
				glFinish();
			}
			else
			{
				break;
			}
		}

	}
	
	// Join all the threads again, blocking main thread
	
#if THREADS > 1
	for(int thread = 0; thread < THREADS-1; thread++)
	{
		pthread_join(threads[thread], NULL);
	}
#endif
	
	QueryPerformanceCounter(&tick_end);
    elapsedTime = (float) (tick_end.QuadPart - tick_start.QuadPart) / frequency.QuadPart;

	printf("Rendering: 100.00%% at %.2f seconds using %d threads and %d SPP.    \n", elapsedTime, THREADS, PIXEL_SAMPLES);

#if RENDERING_STATS
	printf("Rays: %Lu\n", rendering_statistics->rays);
	printf("Box int.: %Lu\n", rendering_statistics->box_intersections);
	printf("Triangle int.: %Lu\n", rendering_statistics->triangle_intersections);
	printf("Box/ray: %.1f\n", (float)rendering_statistics->box_intersections / rendering_statistics->rays);
	printf("Triangle/ray: %.1f\n", (float)rendering_statistics->triangle_intersections / rendering_statistics->rays);
	printf("Render time: %.1f sec\n", elapsedTime);
#endif

}

void Raytracer::onChangeTo()
{
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDrawBuffer(GL_FRONT);
}

void Raytracer::onChangeFrom()
{

}