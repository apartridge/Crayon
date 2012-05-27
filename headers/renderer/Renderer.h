#pragma once

#include "geometry/scene.h"
#include "renderer/camera.h"
#include "renderer/image.h"

class Renderer
{
public:
	virtual void onChangeTo() = 0;
	virtual void onChangeFrom() = 0;
	virtual void drawScene(Scene& scene, Camera& camera, Image* image = 0) = 0;
};