#ifndef PHOTON_TRACER_H
#define PHOTON_TRACER_H

#include "renderer/PhotonMap.h"
#include "geometry/Scene.h"
#include "lights/Light.h"

class PhotonTracer
{
public:
    // Transmit photons from the light sources in the scene, and record in global photon map.
    // TODO: Can also add photon map for caustics only. Need to only tracce photons through specular materals.
    void traceScene(const Scene& scene, int numberOfPhotons);

    // Return photon map generated from traceScene
    PhotonMap* getPhotonMap();

private:
    const Scene* _scene;
    PhotonMap* _photonMap;

    // Generate photons from a light source
    void traceLight(const Light& light, int numberOfPhotons);

    // Trace a photon through the scene
    void PhotonTracer::tracePhoton(Ray& ray);

};

#endif