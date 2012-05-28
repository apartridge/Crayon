#ifndef ASSIGNMENT2H
#define ASSIGNMENT2H

#include <math.h>
#include "renderer/Miro.h"
#include "geometry/Scene.h"
#include "renderer/Camera.h"
#include "renderer/Image.h"
#include "sysutils/Console.h"

#include "lights/PointLight.h"
#include "lights/SquareLight.h"
#include "geometry/TriangleMesh.h"
#include "geometry/Triangle.h"
#include "materials/Lambert.h"
//#include "materials/Shiny.h"
#include "materials/Skydome.h"
#include "geometry/Sphere.h"

void makeTeapotScene();
void makeBunny1Scene();
void makeBunny20Scene();
void makeSponzaScene();
void makeDualScene();
void makeDragonScene();
void makeCornellScene();
void makeCornellSceneGlass();

#endif