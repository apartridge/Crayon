#ifndef CRAYON_SCENE_H_INCLUDED
#define CRAYON_SCENE_H_INCLUDED

#include "../renderer/Miro.h"
#include "../geometry/Object.h"
#include "../lights/Light.h"
#include "../geometry/BVH.h"
#include "../geometry/TriangleMesh.h"

class Camera;
class Image;

class Scene
{
public:
    void addObject(Object* pObj)        {m_objects.push_back(pObj);}
    const Objects* objects() const      {return &m_objects;}


	void addMesh(TriangleMesh* mesh)	 {m_meshes.push_back(mesh);}
    const TriangleMeshes* meshes() const {return &m_meshes;}


    void addLight(Light* pObj)     {m_lights.push_back(pObj);}
    const Lights* lights() const        {return &m_lights;}

    void preCalc();

    bool trace(HitInfo& minHit, Ray& ray,
               float tMin = 0.0f, float tMax = MIRO_TMAX) const;

	BoundingVolumeNode entireScene;

protected:
    Objects m_objects;
    BVH m_bvh;
    Lights m_lights;
	TriangleMeshes m_meshes; // replace for objects?

};

extern Scene * g_scene;

#endif // CRAYON_SCENE_H_INCLUDED
