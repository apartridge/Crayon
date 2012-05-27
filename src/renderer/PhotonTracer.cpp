#include "renderer/PhotonTracer.h"

#include "sysutils/Random.h"


void PhotonTracer::traceScene(const Scene& scene, int numberOfPhotons)
{
    _scene = &scene;
    _photonMap = new PhotonMap(numberOfPhotons);

    const Lights *lightlist = scene.lights();
    const int numberOfLights = lightlist->size();

    Lights::const_iterator it;

    for (it = lightlist->begin(); it != lightlist->end(); ++it)
    {
        const Light* l = *it;
        traceLight(*l, numberOfPhotons / (float)numberOfLights);
    }

    // Balance KD-tree to optimize search
    _photonMap->balance();
}

void PhotonTracer::traceLight(const Light& light, int numberOfPhotons)
{
    for (int i = 0; i < numberOfPhotons; ++i)
    {
        Vector3 pos = light.getPosition();

        // Generate random vector (assume pointlight)
        Vector3 dir;
        do {
            dir.x = Random::uniformRand();
            dir.y = Random::uniformRand();
            dir.z = Random::uniformRand();
        } while (dot(dir, dir) > 1);
        
        Ray ray;
        ray.o = pos;
        ray.d = dir.normalized();
        ray.mediumOfTravel.indexOfRefraction = 1.0;

        tracePhoton(ray);
    }
}

void PhotonTracer::tracePhoton(Ray& ray)
{
    HitInfo hit;
    if (!_scene->trace(hit, ray, epsilon))
        return;
    


}


PhotonMap* PhotonTracer::getPhotonMap()
{
    return this->_photonMap;
}


