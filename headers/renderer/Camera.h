#pragma once

#include "geometry/Vector3.h"
#include "renderer/Miro.h"
#include "geometry/Ray.h"

class OpenGLRenderer;
class Raytracer;

class Camera
{
public:
    Camera();
    virtual ~Camera();

    enum RENDER_MODE
    {
        RENDER_OPENGL   = 0,
        RENDER_RAYTRACE = 1
    };

	enum OPENGL_RENDER_MODE
	{
		OPENGL_RENDER_FILL,
		OPENGL_RENDER_WIRE,
		OPENGL_RENDER_BOUNDINGBOX
	};

    void display(Scene* pScene, Image* pImage);

    RENDER_MODE getRenderer() const {return m_renderer;}

    void setRenderer(RENDER_MODE i) {m_renderer = i;}

	void setOpenGLShowTriad(bool b) {show_triad = b;}
	bool getOpenGLShowTriad() {return show_triad;}
	
	
	void setOpenGLRendererMode(OPENGL_RENDER_MODE mode)
	{
		m_openGlRenderMode = mode;
	}

	OPENGL_RENDER_MODE openGLRenderMode()
	{
		return m_openGlRenderMode;
	}

    void setEye(float x, float y, float z);
    void setEye(const Vector3& eye);
    void setUp(float x, float y, float z);
    void setUp(const Vector3& up);
    void setViewDir(float x, float y, float z);
    void setViewDir(const Vector3& vd);
    void setLookAt(float x, float y, float z);
    void setLookAt(const Vector3& look);
    void setBGColor(float x, float y, float z);
    void setBGColor(const Vector3& color);
    void setFOV(float fov) {m_fov = fov;}

	void setFocalLength(float focalLength)
	{
		m_focalLength = focalLength;
	}
	void setAperture(float aperture)
	{
		m_aperture = aperture;
	}

	void resetPosition();

    float fov() const                {return m_fov;}
    const Vector3 & viewDir() const  {return m_viewDir;}
    const Vector3 & lookAt() const   {return m_lookAt;}
    const Vector3 & up() const       {return m_up;}
    const Vector3 & eye() const      {return m_eye;}
    const Vector3 & bgColor() const  {return m_bgColor;}

	float aperture() const {return m_aperture;}
	float focalLength() const {return m_focalLength;}

    Ray eyeRay(float x, float y, int imageWidth, int imageHeight);

	Vector3 pointInFocus() const;
    
private:

    void calcLookAt();

    Vector3 m_bgColor;
    RENDER_MODE m_renderer;
	OPENGL_RENDER_MODE m_openGlRenderMode;

	Vector3 m_eyeDefault;
	bool m_eyeDefaultSet;
	Vector3 m_lookAtDefault;
	bool m_lookAtDefaultSet;

    // main screen params
    Vector3 m_eye;
    Vector3 m_up;
    Vector3 m_viewDir;
    Vector3 m_lookAt;
    float m_fov;
	bool show_triad;

	float m_focalLength;
	float m_aperture;

	// Renderers
	OpenGLRenderer* m_openGLRenderer;
	Raytracer* m_raytracer;

};

extern Camera * g_camera;

inline void Camera::setEye(float x, float y, float z)
{
    m_eye.set(x, y, z);
	if(!m_eyeDefaultSet)
	{
		m_eyeDefault.set(x,y,z);
		m_eyeDefaultSet = true;
	}
}

inline void Camera::setEye(const Vector3& eye)
{
    setEye(eye.x, eye.y, eye.z);
}

inline void Camera::setUp(float x, float y, float z)
{
    m_up.set(x, y, z);
    m_up.normalize();
}

inline void Camera::setUp(const Vector3& up)
{
    setUp(up.x, up.y, up.z);
}

inline void Camera::setViewDir(float x, float y, float z)
{
    m_viewDir.set(x, y, z);
    m_viewDir.normalize();
}

inline void Camera::setViewDir(const Vector3& vd)
{
    setViewDir(vd.x, vd.y, vd.z);
}

inline void Camera::setLookAt(float x, float y, float z)
{
    Vector3 dir = Vector3(x, y, z) - m_eye;
    setViewDir(dir);
	if(!m_lookAtDefaultSet)
	{
		m_lookAtDefault.set(x,y,z);
		m_lookAtDefaultSet = true;
	}
}

inline void Camera::setLookAt(const Vector3& vd)
{
    setLookAt(vd.x, vd.y, vd.z);
}







inline void Camera::setBGColor(float x, float y, float z)
{
    m_bgColor.set(x, y, z);
}

inline void Camera::setBGColor(const Vector3& vd)
{
    setBGColor(vd.x, vd.y, vd.z);
}
