#pragma once

/*
#include <GL\glut.h>
*/

#include "glut.h"
#include "renderer.h"

#include "geometry/Vector3.h"
#include "Camera.h"
#include "geometry/BoundingBox.h"

class OpenGLRenderer : public Renderer
{
public:
	OpenGLRenderer();

	virtual void onChangeTo();
	virtual void onChangeFrom();

	virtual void drawScene(Scene& scene, Camera& camera, Image* image = NULL);

	void axis3D();
	void triad3D();
	void cube(BoundingBox&);
	void drawVolumeNode(BoundingVolumeNode&, int);

};