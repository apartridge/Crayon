#ifndef CSE168_SKYDOME_H_INCLUDED
#define CSE168_SKYDOME_H_INCLUDED

#include "Material.h"

class Skydome : public Material
{
public:
	Skydome(const char* filename);

	void sphereCoordinates(Vector3 ray, int* res) const;

    virtual ~Skydome();

    virtual void preCalc() {}
    
    virtual Vector3 shade(const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const;

protected:
	Vector3* image;
	int width;
	int height;
};

#endif
