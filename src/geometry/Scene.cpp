#include <time.h>

#include "renderer/Miro.h"
#include "geometry/Scene.h"
#include "renderer/Camera.h"
#include "renderer/Image.h"
#include "sysutils/Console.h"
#include "renderer/PhotonTracer.h"
#include "sysutils/PerformanceTimer.h"

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

    printf("Tracing %d photons\n", PhotonTracer::DefaultNumberOfPhotons);
    PerformanceTimer timer;
    timer.start();

    PhotonTracer* phTracer = new PhotonTracer();
    phTracer->traceScene(*this, PhotonTracer::DefaultNumberOfPhotons);
    this->m_phmap = phTracer->getPhotonMap();

    timer.stop();
    printf("Photon tracing done in %f seconds\n", timer.elapsedSec());
}

bool Scene::trace(HitInfo& minHit, Ray& ray, float tMin, float tMax) const
{
    return m_bvh.intersect(minHit, ray, tMin, tMax);
}
