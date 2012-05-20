#include "../../headers/geometry/BoundingBox.h"

BoundingBox::BoundingBox()
{
	min = Vector3(0);
	max = Vector3(0);
}

BoundingBox::BoundingBox(Vector3 _min, Vector3 _max)
{
	min = _min;
	max = _max;
}

void BoundingBox::setMin(Vector3 a)
{
	min = a;
}

void BoundingBox::setMax(Vector3 a)
{
	max = a;
}

bool BoundingBox::contains(Vector3& point) const
{
	return point.x >= min.x && point.x <= max.x
			&& point.y >= min.y && point.y <= max.y
			&& point.z >= min.z && point.z <= max.z;
}

bool BoundingBox::contains(BoundingBox& box) const
{
	return box.min.x >= this->min.x &&
		   box.min.y >= this->min.y &&
		   box.min.z >= this->min.z &&
		   box.max.x <= this->max.x &&
		   box.max.y <= this->max.y &&
		   box.max.z <= this->max.z;
}

void BoundingBox::expand(const BoundingBox& other)
{

	if(this->max.x < other.max.x) this->max.x = other.max.x;
	if(this->max.y < other.max.y) this->max.y = other.max.y;
	if(this->max.z < other.max.z) this->max.z = other.max.z;

	if(this->min.x > other.min.x) this->min.x = other.min.x;
	if(this->min.y > other.min.y) this->min.y = other.min.y;
	if(this->min.z > other.min.z) this->min.z = other.min.z;
}

float BoundingBox::surfaceArea() const
{

	float sum = 2*( max.x*max.z - max.x*min.z - min.x*max.z + min.x*min.z
					+ max.y*max.x - max.y*min.x - min.y*max.x + min.y*min.x
					+ max.z*max.y - max.z*min.y - min.z*max.y + min.z*min.y);

	return sum;
}

void BoundingBox::print() const
{
	printf("BB%x Min(%.2f, %.2f, %.2f) Max(%.2f, %.2f, %.2f)\n",
		  this, min.x, min.y, min.z, max.x, max.y, max.z);
}


/*
// Ideas from An Efﬁcient and Robust Ray–Box Intersection Algorithm
// http://people.csail.mit.edu/amy/papers/box-jgt.pdf
*/

inline void swap(float& a, float&b)
{
	float temp = a;
	a = b;
	b = temp;
}

bool BoundingBox::intersectedByRay(const Ray& ray, float tMinG, float tMaxG) const
{
	float divx = 1 / ray.d.x;
	float tmin = (min.x - ray.o.x) * divx;
	float tmax = (max.x - ray.o.x) * divx;

	if(divx < 0)
	{
		swap(tmin, tmax);
	}

	if(tmax < 0)
	{
		//return false;
	}

	float divy =  1 / ray.d.y;
	float tminy = (min.y - ray.o.y) * divy;
	float tmaxy = (max.y - ray.o.y) * divy;

	if(divy  < 0)
	{
		swap(tminy, tmaxy);
	}

	if(tmin > tmaxy || tmax < tminy)
	{
		return false;
	}

	if(tminy > tmin)
	{
		tmin = tminy;
	}

	if(tmaxy < tmax)
	{
		tmax = tmaxy;
	}

	if(tmax < 0)
	{
		//return false;
	}

	float divz = 1 / ray.d.z;
	float tminz = (min.z - ray.o.z) * divz;
	float tmaxz = (max.z - ray.o.z) * divz;

	if(divz <  0)
	{
		swap(tminz, tmaxz);
	}

	if(tmin > tmaxz || tmax < tminz)
	{
		return false;
	}

	if(tminz > tmin)
	{
		tmin = tminz;
	}

	if(tmaxz < tmax)
	{
		tmax = tmaxz;
	}

	return tmax >= tMinG && tmin <= tMaxG;

}