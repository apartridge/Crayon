#ifndef CRAYON_TRIANGLE_H_INCLUDED
#define CRAYON_TRIANGLE_H_INCLUDED

#include "Object.h"
#include "BoundingBox.h"

/*
    The Triangle class stores a pointer to a mesh and an index into its
    triangle array. The mesh stores all data needed by this Triangle.
*/
class Triangle : public Object
{
public:
    Triangle(TriangleMesh * m = 0, unsigned int i = 0, Vector3 gNormal = 0);
    virtual ~Triangle();

    void setIndex(unsigned int i) {m_index = i;}
    void setMesh(TriangleMesh* m) {m_mesh = m;}

	TriangleMesh* getMesh() const { return m_mesh; }
	unsigned int getIndex(){ return m_index; }

	void calculateBoundingBox();
	
    virtual void renderGL();
    virtual bool intersect(HitInfo& result, Ray& ray,
                           float tMin = 0.0f, float tMax = MIRO_TMAX);


protected:
    TriangleMesh* m_mesh;
    unsigned int m_index;
	Vector3 m_gNormal;
};

#endif // CRAYON_TRIANGLE_H_INCLUDED
