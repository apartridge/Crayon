#include "renderer/Window.h"
#include "renderer/glut.h"
#include "renderer/Miro.h"
#include "renderer/Camera.h"
#include "renderer/Image.h"
#include <stdlib.h>
#include <time.h>

#define ANGFACT     1.0
#define LEFT        4
#define MIDDLE      2
#define RIGHT       1

#ifdef WIN32
// disable useless warnings
#pragma warning(disable:4996)
#endif

namespace
{

	// Non-member functions used as proxy callbacks to our real C++ member functions
	Window *g_Window;
	void display() {g_Window->display();}
	void resize(int x,int y) {g_Window->reshape(x,y);}
	void keyboard(unsigned char key, int x, int y) {g_Window->keyboard(key,x,y);}
	void mouse(int btn,int state,int x,int y) {g_Window->mouse(btn,state,x,y);}
	void motion(int x, int y) {g_Window->motion(x,y);}

}

int show_level = 0;
int levels[100];
Vector3 pointInFocusLocked;
bool pointInFocusShow;


Window::Window(int * argc, char* argv[]) :
    m_scaleFact(0.4f),
    m_activeButton(0),
    m_mouseX(0),
    m_mouseY(0)
{
    // Initialize GLUT
    glutInit(argc, argv);

    // Create the window
    glutInitWindowSize(g_image->width(), g_image->height());
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Crayon");
}


void Window::mainLoop()
{
    // Setup callback functions
    g_Window = this;
    glutDisplayFunc(::display);
    glutKeyboardFunc(::keyboard);
    glutMouseFunc(::mouse);
    glutMotionFunc(::motion);
    glutReshapeFunc(::resize);

	pointInFocusLocked = g_camera->pointInFocus();
    
    // Start the glut main loop, never returns
    glutMainLoop();
}


void Window::display()
{
    g_camera->display(g_scene, g_image); // take a snapshot of the scene

    glFinish(); // flush the openGL pipeline
}


void Window::motion(int x, int y)
{
    int dx, dy;     // change in mouse coordinates

    dx = x - m_mouseX;     // change in mouse coords
    dy = y - m_mouseY;

    if (m_activeButton & LEFT)
    {
        float xfact = -ANGFACT*dy;
        float yfact = -ANGFACT*dx;
        // construct a coordinate system from up and viewdir
        Vector3 vRight = cross(g_camera->viewDir(), g_camera->up());
        // now rotate everything
        Vector3 v = g_camera->viewDir();
        v.rotate(xfact*PI/180., vRight);
        v.rotate(yfact*PI/180., g_camera->up());
        g_camera->setViewDir(v);
    }

    m_mouseX = x;          // new current position
    m_mouseY = y;

    glutPostRedisplay();
}


void Window::mouse(int button, int state, int x, int y)
{
	if(g_camera->getRenderer() == Camera::RENDER_RAYTRACE)
	{
		return;
	}


    int b; // LEFT, MIDDLE, or RIGHT

    switch (button)
    {
        case GLUT_LEFT_BUTTON:
            b = LEFT;
        break;
        
        case GLUT_MIDDLE_BUTTON:
            b = MIDDLE;
        break;
        
        case GLUT_RIGHT_BUTTON:
            b = RIGHT;
        break;
        
        default:
            b = 0;
    }

    if (state == GLUT_DOWN)
    {
        m_mouseX = x;
        m_mouseY = y;
        m_activeButton |= b;       /* set the proper bit   */
    }
    else
        m_activeButton &= ~b;      /* clear the proper bit */
}

void Window::keyboard(unsigned char key, int x, int y)
{

    switch (key)
    {
        case 27:
            exit(0);
        break;

        case 'i':
        case 'I':
        {
            char str[1024];
            sprintf(str, "miro_%d.ppm", time(0));
			if (g_camera->getRenderer() == Camera::RENDER_OPENGL)
            {
                unsigned char* buf = new unsigned char[g_image->width()*g_image->height()*3];
                glReadPixels(0, 0, g_image->width(), g_image->height(),
                             GL_RGB, GL_UNSIGNED_BYTE, buf);
                g_image->writePPM(str, buf, g_image->width(), g_image->height());
            }
            else
            {
                g_image->writePPM(str);
            }
            break;
        }

        case 'r':
        case 'R':
            g_camera->setRenderer(Camera::RENDER_RAYTRACE);
			this->setWindowTitle("Crayon - Raytracer Mode");
        break;

        case 'g':
        case 'G':
            g_camera->setRenderer(Camera::RENDER_OPENGL);
			g_camera->setOpenGLRendererMode(Camera::OPENGL_RENDER_WIRE);
			this->setWindowTitle("Crayon - Wireframe Mode");
        break;

		case 'f':
        case 'F':
            g_camera->setRenderer(Camera::RENDER_OPENGL);
			g_camera->setOpenGLRendererMode(Camera::OPENGL_RENDER_FILL);
			this->setWindowTitle("Crayon - Preview Mode");
        break;

		case 'b':
        case 'B':
            g_camera->setRenderer(Camera::RENDER_OPENGL);
			g_camera->setOpenGLRendererMode(Camera::OPENGL_RENDER_BOUNDINGBOX);
			this->setWindowTitle("Crayon - Bounding Boxes");
        break;


		case 't':
        case 'T':
			g_camera->setRenderer(Camera::RENDER_OPENGL);
            g_camera->setOpenGLShowTriad(!g_camera->getOpenGLShowTriad());
			this->setWindowTitle("Crayon - Preview Mode");
        break;

        case '+':
            m_scaleFact *= 1.5;
        break;

        case '-':
            m_scaleFact /= 1.5;
        break;

        case 'w':
        case 'W':
            g_camera->setEye(g_camera->eye() + m_scaleFact*g_camera->viewDir());
        break;

        case 's':
        case 'S':
            g_camera->setEye(g_camera->eye() - m_scaleFact*g_camera->viewDir());
        break;

        case 'q':
        case 'Q':
            g_camera->setEye(g_camera->eye() + m_scaleFact*g_camera->up());
        break;

        case 'z':
        case 'Z':
            g_camera->setEye(g_camera->eye() - m_scaleFact*g_camera->up());
        break;

        case 'a':
        case 'A':
        {
            Vector3 vRight = cross(g_camera->viewDir(), g_camera->up());
            g_camera->setEye(g_camera->eye() - m_scaleFact*vRight);
            break;
        }

        case 'd':
        case 'D':
        {
            Vector3 vRight = cross(g_camera->viewDir(), g_camera->up());
            g_camera->setEye(g_camera->eye() + m_scaleFact*vRight);
            break;
        }

		case 'p':
		{
			show_level++;
			//printf("Showing level %d\n", show_level);
			break;
		}

		case 'o':
		{
			show_level--;
			//printf("Showing level %d\n", show_level);
			break;
		}

		// Press L to lock the point in focus so it can be examined in OPENGL preview mode
		case 'l':
		{
			pointInFocusLocked = g_camera->pointInFocus();
			break;
		}

		case 'k':
		{
			pointInFocusShow = !pointInFocusShow;
			break;
		}

		case '1':
		{
			levels[show_level] = 1;
			break;
		}

		case '2':
		{
			levels[show_level] = 2;
			break;
		}

    }

	/*printf("Level: %d\n", show_level);
	for(int i = 0; i <= show_level; i++)
	{
		printf("  Level %d is %d\n", i, levels[i]);
	}
	printf("\n\n");*/


    glutPostRedisplay();
}


void Window::reshape(int w, int h)
{
    g_image->resize(w, h);
    glViewport(0, 0, w, h);
    g_camera->setRenderer(Camera::RENDER_OPENGL);
	g_camera->setOpenGLRendererMode(Camera::OPENGL_RENDER_FILL);
    glutPostRedisplay();
}

void Window::setWindowTitle(char* subtitle)
{
	glutSetWindowTitle(subtitle);
}