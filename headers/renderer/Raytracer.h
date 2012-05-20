#pragma once

#include "../../headers/renderer/renderer.h"


class Raytracer : Renderer
{
public:
	Raytracer();
	virtual void onChangeTo();
	virtual void onChangeFrom();
	virtual void drawScene(Scene& scene, Camera& camera, Image* image = NULL);
};

