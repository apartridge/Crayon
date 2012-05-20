#pragma once

#include "../../headers/geometry/scene.h"
#include "../../headers/renderer/camera.h"
#include "../../headers/renderer/image.h"

class Renderer
{
public:
	virtual void onChangeTo() = 0;
	virtual void onChangeFrom() = 0;
	virtual void drawScene(Scene& scene, Camera& camera, Image* image = 0) = 0;
};