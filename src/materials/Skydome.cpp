#include "Skydome.h"
#include "Ray.h"
#include "Scene.h"
#include "PFMLoader.h"
#include <iostream>


Skydome::Skydome(const char* filename) :
			Material(Vector3(0,0.3,0), Medium(1), 0)
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

Vector3 Skydome::shade(const Ray& ray, const HitInfo& hit, const Scene& scene, float contributionFactor, const int depth) const
{

	
	int uvCoordinatesNormalized[2];
	sphereCoordinates(ray.d, uvCoordinatesNormalized);



	// Look up in the skydome!
	return contributionFactor*image[uvCoordinatesNormalized[0]*width + uvCoordinatesNormalized[1]];
}
