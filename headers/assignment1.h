#pragma once

#include <math.h>
#include "../headers/renderer/Miro.h"
#include "../headers/geometry/Scene.h"
#include "../headers/renderer/Camera.h"
#include "../headers/renderer/Image.h"
#include "../headers/sysutils/Console.h"

#include "../headers/lights/PointLight.h"
#include "../headers/geometry/TriangleMesh.h"
#include "../headers/geometry/Triangle.h"
#include "../headers/materials/Lambert.h"
#include "../headers/materials/Shiny.h"
#include "../headers/materials/Skydome.h"
#include "../headers/geometry/Sphere.h"

void makeBunnyScene();
void makeSphereScene();
void makeTeapotScene();