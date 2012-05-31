#include <stdio.h>
#include <stdlib.h>
#include "renderer/Miro.h"
#include "renderer/Camera.h"
#include "renderer/Image.h"
#include "geometry/Scene.h"
#include "sysutils/Console.h" 
#include "renderer/glut.h"
#include "renderer/openglrenderer.h"
#include "renderer/raytracer.h"
#include "renderer/RenderingStats.h"
#include "sysutils/Random.h"

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
    m_fov((45.)*(PI/180.)),
	m_focalLength(0),
	m_aperture(0.0)
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


	Vector3 direction = (imPlaneUPos*uDir + imPlaneVPos*vDir - wDir).normalize();
	Vector3 origin = m_eye;

	// Depth of Field
	//

	if(m_focalLength > 0)
	{

		// Find a point Xf on our focus plane
		// Xf together with viewDir defines focus plane

		Vector3 Xf = pointInFocus();

		//float t = m_focalLength;
		float t = (dot(m_viewDir, Xf) - dot(m_viewDir, m_eye) ) / dot(m_viewDir, direction); 

		// Point on focus plane
		Vector3 X = m_eye + direction * t; 

		// We move our camera location somewhat. Send new ray thru X

		float u, v;
		do {
			u = Random::uniformRand();
			v = Random::uniformRand();
		}
		while (u*u + v*v > 1);

		origin = m_eye + (uDir*u + vDir*v)*m_aperture;
		direction = (X-origin).normalized();
	}

    return Ray(origin, direction, Medium(1));
}


// The point in focus on the line from the eye through the center of the lens

Vector3 Camera::pointInFocus() const
{
	return m_eye + m_viewDir*m_focalLength; 
}