#include "renderer/PhotonTracer.h"

#include "sysutils/Random.h"
#include "renderer/glut.h"
#include "sysutils/PerformanceTimer.h"

void PhotonTracer::traceScene(const Scene& scene, int numberOfPhotons)
{
    _scene = &scene;
    _photonMap = new PhotonMap(numberOfPhotons);

    const Lights *lightlist = scene.lights();
    const int numberOfLights = lightlist->size();

    Lights::const_iterator it;

    for (it = lightlist->begin(); it != lightlist->end(); ++it)
    {
        const Light* l = *it;
        const int numEmittedPhotons = numberOfPhotons / numberOfLights;
        traceLight(*l, numEmittedPhotons);
    }

    _photonMap->balance();
}

// Like reference implementation of emit_photons_from_diffuse_point_light as described
// in "Realistic Image Synthesis using Photon Mapping" page 57.
void PhotonTracer::traceLight(const Light& light, int numberOfPhotons)
{
    int n = 0, c = 0;

	PerformanceTimer pt;
	pt.start();
    
	while (n < numberOfPhotons)
    {
        //Vector3 dir = light.emitPhoton();
        //Vector3 power = light.power() * light.color();
        //Ray ray(light.getPosition(), dir);

        PhotonRay photon = light.emitPhoton();
        Ray ray(photon.origin, photon.direction);

        int photons = tracePhoton(ray, photon.power, 0);
		n += photons;
		c += photons;

		if(c > numberOfPhotons/50)
		{
			c = 0;
			pt.stop();
			printf("%.2f seconds. %.2f %% done. Remaining: %.2f sec.\r",
				pt.elapsedSec(),
				(n/float(numberOfPhotons))*100.f, pt.elapsedSec()*(float(numberOfPhotons)/float(n) - 1));
			pt.start();
		}

    }
    _photonMap->scale_photon_power(1 / (float)numberOfPhotons); // Maybe it should be 'n' here?
}

int PhotonTracer::tracePhoton(const Ray& ray, Vector3 power, int bounce)
{
    if (bounce > MaxPhotonBounces)
	{
        return 0;
	}

    HitInfo hit;
    Ray mutable_ray (ray.origin(), ray.direction()); // Bug: Scene::trace should have const Ray&
    if (!_scene->trace(hit, mutable_ray, epsilon))
	{
        return 0;
	}

    const Material* mat = hit.material;

    // Russian roulette on what to do with photon
    float rd_avg = (mat->diffuseColor(hit).x + mat->diffuseColor(hit).y + mat->diffuseColor(hit).z) / 3.0;
    float rs_avg = (mat->Rs().x + mat->Rs().y + mat->Rs().z) / 3.0;
    float rt_avg = (mat->Rt().x + mat->Rt().y + mat->Rt().z) / 3.0;

    //assert(rd_avg + rs_avg + rt_avg <= 1.0 + epsilon);

    const float e = Random::uniformRand();

    int stored = 0;
    
    // Hit a diffuse surface
    if (rd_avg > epsilon)
    {
        // Abort if hitting from behind
        if (dot(ray.direction(), hit.N) > 0)
            return 0;
        
        // Don't store the first bounce (only want indirect illumination)
        if (bounce > 0)
        {
            _photonMap->store(&power[0], &hit.P[0], &ray.direction()[0]);
            stored += 1;
        }
    }

    // Diffuse reflection
    if (e < rd_avg)
    {
        power = (power * mat->diffuseColor(hit)) / rd_avg;

        Vector3 n = hit.N;
	    Vector3 u = n.perpendicular();
        Vector3 v = cross(n, u).normalized();

	    const float phi = 2*PI*Random::uniformRand();
        const float theta = asin(sqrt(Random::uniformRand()));

        Vector3 d = (cos(phi) * sin(theta) * u + 
                     sin(phi) * sin(theta) * v +
                     cos(theta) * n).normalized();

        Ray reflect (hit.P, d);
        reflect.mediumOfTravel.indexOfRefraction = ray.mediumOfTravel.indexOfRefraction;

        stored += tracePhoton(reflect, power, bounce + 1);
    }

    // Specular reflection
    else if (e < rs_avg + rd_avg)
    {
        power = (power * mat->Rs()) / rs_avg;

        Ray reflect (hit.P, Material::reflect(ray, hit));
        reflect.mediumOfTravel.indexOfRefraction = ray.mediumOfTravel.indexOfRefraction;

        stored += tracePhoton(reflect, power, bounce + 1);
    }

    // Transmission
    else if (e < rt_avg + rs_avg + rd_avg)
    {
        power = (power * mat->Rt()) / rt_avg;

        Ray refract = mat->refractRay(ray, hit);

        if (refract.direction() != Vector3(0.0))
            stored += tracePhoton(refract, power, bounce + 1);
    }

    // Absorb
    else
    {
        // Do nothing, photon is reduceed to heat :(
    }

    return stored;
}

PhotonMap* PhotonTracer::getPhotonMap() const
{
    return _photonMap;
}

void PhotonMap::renderGL()
{
    glBegin(GL_POINTS);
    for (int i = 0; i < stored_photons; ++i)
    {
        Photon ph = getPhoton(i);
        glVertex3f(ph.pos[0], ph.pos[1], ph.pos[2]);
    }
    glEnd();
}
