#include "renderer/PhotonTracer.h"

#include "sysutils/Random.h"
#include "renderer/glut.h"

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
        const int numEmittedPhotons = numberOfPhotons / numberOfLights;
        traceLight(*l, numEmittedPhotons);
        _photonMap->scale_photon_power(1 / (float)numEmittedPhotons); 
    }

    _photonMap->balance();
}

// Like reference implementation of emit_photons_from_diffuse_point_light as described
// in "Realistic Image Synthesis using Photon Mapping" page 57.
void PhotonTracer::traceLight(const Light& light, int numberOfPhotons)
{
    int n = 0;
    while (n < numberOfPhotons)
    {
        Vector3 dir = light.emitPhoton();
        Vector3 power = light.power() * light.color();
        Ray ray(light.getPosition(), dir);

        if (tracePhoton(ray, power, 0))
			n++;
    }
}

bool PhotonTracer::tracePhoton(const Ray& ray, Vector3 power, int bounce)
{
    if (bounce > MaxPhotonBounces)
	{
        return false;
	}

    HitInfo hit;
    Ray mutable_ray (ray.origin(), ray.direction()); // Bug: Scene::trace should have const Ray&
    if (!_scene->trace(hit, mutable_ray, epsilon))
	{
        return false;
	}

    const Material* mat = hit.material;

    // Russian roulette on what to do with photon
    float rd_avg = (mat->Rd().x + mat->Rd().y + mat->Rd().z) / 3.0;
    float rs_avg = (mat->Rs().x + mat->Rs().y + mat->Rs().z) / 3.0;
    float rt_avg = (mat->Rt().x + mat->Rt().y + mat->Rt().z) / 3.0;

    const float e = Random::uniformRand();
    
    // Diffuse reflection
    if (e < rd_avg)
    {
        power = (power * mat->Rd()) / rd_avg;

        Vector3 n = hit.N;
	    Vector3 u = n.perpendicular();
        Vector3 v = cross(n, u).normalized();

	    const float phi = 2*PI*Random::uniformRand();
        const float theta = asin(sqrt(Random::uniformRand()));

        Vector3 d = (cos(phi) * sin(theta) * u + 
                     sin(phi) * sin(theta) * v +
                     cos(theta) * n).normalized();

        Ray reflect (hit.P, d);
        //reflect.d = d;
        reflect.mediumOfTravel.indexOfRefraction = ray.mediumOfTravel.indexOfRefraction;
       // reflect.o = hit.P;

        return tracePhoton(reflect, power, bounce + 1);
    }

    // Specular reflection
    else if (e < rs_avg + rd_avg)
    {
        power = (power * mat->Rs()) / rs_avg;

        Ray reflect (hit.P, Material::reflect(ray, hit));
        //reflect.o = hit.P;
        reflect.mediumOfTravel.indexOfRefraction = ray.mediumOfTravel.indexOfRefraction;
        //reflect.d = Material::reflect(ray, hit);

        return tracePhoton(reflect, power, bounce + 1);
    }

    // Transmission
    else if (e < rt_avg + rs_avg + rd_avg)
    {
        power = (power * mat->Rt()) / rt_avg;

        Ray refract = mat->refractRay(ray, hit);

        if (refract.direction() != Vector3(0.0))
            return tracePhoton(refract, power, bounce + 1);
        else
            return 0;
    }

    // Absorb
    else
    {
        _photonMap->store(&power[0], &hit.P[0], &ray.direction()[0]);
        return 1;
    }
}

PhotonMap* PhotonTracer::getPhotonMap() const
{
    return _photonMap;
}

void PhotonMap::renderGL()
{
    glBegin(GL_POINTS);
    for (int i = 0; i < stored_photons; ++i)
    {
        Photon ph = getPhoton(i);
        glVertex3f(ph.pos[0], ph.pos[1], ph.pos[2]);
    }
    glEnd();
}
