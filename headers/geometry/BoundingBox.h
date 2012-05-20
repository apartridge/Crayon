#pragma once

#include "Vector3.h"
#include "Ray.h"

#define BOUNDINGBOX_PADDING_DELTA 0.0001

class BoundingBox
{
public:
	Vector3 min;
	Vector3 max;

public:
	void setMin(Vector3);
	void setMax(Vector3);
	BoundingBox();
	BoundingBox(Vector3, Vector3);
	
	//BoundingBox bbUnion(const BoundingBox& other) const;
	void expand(const BoundingBox& other);

	const Vector3 & getMin() const  {return min;}
	const Vector3 & getMax() const  {return max;}

	bool contains(Vector3&) const;
	bool contains(BoundingBox&) const;
	bool intersectedByRay(const Ray& ray, float tMinG, float tMaxG) const;

	float surfaceArea() const;

	Vector3 midpoint() const {
		return Vector3 (  min.x + (max.x-min.x)/2.0f,
						  min.y + (max.x-min.x)/2.0f, 
						  min.z + (max.x-min.x)/2.0f )  ;
	}

	void print() const;
};
