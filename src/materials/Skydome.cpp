#include <iostream>

#include "materials/Skydome.h"
#include "geometry/Ray.h"
#include "geometry/Scene.h"
#include "sysutils/PFMLoader.h"

Skydome::Skydome(const char* filename) : Material()
{
	this->image = readPFMImage(filename, &this->width, &this->height);
}

Skydome::~Skydome()
{
	delete this->image;
}

void Skydome::sphereCoordinates(Vector3 raydir, int* res) const
{
	float r = (1/3.141592)*acos(raydir.z)/sqrt(raydir.x*raydir.x + raydir.y*raydir.y);
	res[0] = (raydir.x*r+1)/2 * width,
	res[1] = (raydir.y*r+1)/2 * height;
}

Vector3 Skydome::shade(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{
	int uvCoordinatesNormalized[2];
	
	sphereCoordinates(ray.direction(), uvCoordinatesNormalized);

	/*if(uvCoordinatesNormalized[0] >= height)
	{
		printf("UV0 too big! Skydome.cpp\n");
		return 0;
	}
		

	if(uvCoordinatesNormalized[1] >= width){
		printf("UV1 too big! Skydome.cpp\n");
		return 0;
	}
	*/

	// This overflows all the time?
	return Vector3(1,1,1);


	return (1 * image[uvCoordinatesNormalized[0]*width + uvCoordinatesNormalized[1]]).saturated();
}
