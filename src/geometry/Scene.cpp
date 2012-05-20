#include "../../headers/renderer/Miro.h"
#include "../../headers/geometry/Scene.h"
#include "../../headers/renderer/Camera.h"
#include "../../headers/renderer/Image.h"
#include "../../headers/sysutils/Console.h"
#include <time.h>

Scene * g_scene = 0;

void Scene::preCalc()
{
    Objects::iterator it;
    for (it = m_objects.begin(); it != m_objects.end(); it++)
    {
        Object* pObject = *it;
        pObject->preCalc();
    }
    Lights::iterator lit;
    for (lit = m_lights.begin(); lit != m_lights.end(); lit++)
    {
        Light* pLight = *lit;
        pLight->preCalc();
    }

	m_bvh.build(&m_objects, entireScene);
}

bool Scene::trace(HitInfo& minHit, Ray& ray, float tMin, float tMax) const
{
    return m_bvh.intersect(minHit, ray, tMin, tMax);
}
