#ifndef PHOTON_TRACER_H
#define PHOTON_TRACER_H

#include "renderer/PhotonMap.h"
#include "geometry/Scene.h"

class PhotonTracer
{
public:
    // Shoot out photons from the light sources in the scene, and record in photon map
    void traceScene(const Scene& scene, unsigned long numberOfPhotons);

    // Return photon map generated from traceScene
    PhotonMap* getPhotonMap();

private:
    PhotonMap* photonMap;

};

#endif