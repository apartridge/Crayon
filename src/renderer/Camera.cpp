#include <stdio.h>
#include <stdlib.h>
#include "../../headers/renderer/Miro.h"
#include "../../headers/renderer/Camera.h"
#include "../../headers/renderer/Image.h"
#include "../../headers/geometry/Scene.h"
#include "../../headers/sysutils/Console.h" 
#include "../../headers/renderer/glut.h"
#include "../../headers/renderer/openglrenderer.h"
#include "../../headers/renderer/raytracer.h"
#include "../../headers/renderer/RenderingStats.h"

Camera * g_camera = 0;
#if RENDERING_STATS
RenderingStats* rendering_statistics = 0;
#endif

static bool firstRayTrace = true; 
const float HalfDegToRad = DegToRad/2.0f;

Camera::Camera() :
    m_bgColor(0,0,0),
    m_renderer(RENDER_OPENGL),
    m_eye(0,0,0),
    m_viewDir(0,0,-1),
    m_up(0,1,0),
    m_lookAt(FLT_MAX, FLT_MAX, FLT_MAX),
    m_fov((45.)*(PI/180.))
{
    calcLookAt();
	m_openGLRenderer = new OpenGLRenderer();
	m_raytracer = new Raytracer();
}


Camera::~Camera()
{

}

void Camera::display(Scene* pScene, Image* pImage)
{
    calcLookAt();
#if RENDERING_STATS
	rendering_statistics = new RenderingStats;
#endif
   
    if (m_renderer == RENDER_OPENGL)
    {
		pImage->clear(bgColor());
		m_openGLRenderer->onChangeTo();
		m_openGLRenderer->drawScene(*pScene, *this, pImage );
        firstRayTrace = true;
    }
    else if (m_renderer == RENDER_RAYTRACE)
    {
        if (firstRayTrace)
        {
			m_raytracer->onChangeTo();
			m_raytracer->drawScene(*pScene, *this, g_image);
            firstRayTrace = false;
        }
        
        g_image->draw();
    }
#if RENDERING_STATS
	delete rendering_statistics;
#endif
}


void Camera::calcLookAt()
{
    // this is true when a "lookat" is not used in the config file
    if (m_lookAt.x != FLT_MAX)
    {
        setLookAt(m_lookAt);
        m_lookAt.set(FLT_MAX, FLT_MAX, FLT_MAX);
    }
}


Ray Camera::eyeRay(float x, float y, int imageWidth, int imageHeight)
{
    // first compute the camera coordinate system 
    // ------------------------------------------

    // wDir = e - (e+m_viewDir) = -m_vView
    const Vector3 wDir = Vector3(-m_viewDir).normalize(); 
    const Vector3 uDir = cross(m_up, wDir).normalize(); 
    const Vector3 vDir = cross(wDir, uDir);    



    // next find the corners of the image plane in camera space
    // --------------------------------------------------------

    const float aspectRatio = (float)imageWidth/(float)imageHeight; 


    const float top     = tan(m_fov*HalfDegToRad); 
    const float right   = aspectRatio*top; 

    const float bottom  = -top; 
    const float left    = -right; 



    // transform x and y into camera space 
    // -----------------------------------

    const float imPlaneUPos = left   + (right - left)*(((float)x+0.5f)/(float)imageWidth); 
    const float imPlaneVPos = bottom + (top - bottom)*(((float)y+0.5f)/(float)imageHeight); 

    return Ray(m_eye, (imPlaneUPos*uDir + imPlaneVPos*vDir - wDir).normalize(), Medium(1));
}
