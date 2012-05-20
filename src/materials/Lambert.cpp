#include "../../headers/materials/Lambert.h"
#include "../../headers/geometry/Ray.h"
#include "../../headers/geometry/Scene.h"

Lambert::Lambert(const Vector3 & kd,  float opacity, const Vector3 & ka) :
    m_ka(ka), Material(kd, Medium(1), opacity)
{

}

Lambert::~Lambert()
{
}

Vector3 Lambert::shade(const Ray& ray, const HitInfo& hit, const Scene& scene, float alpha, const int depth) const
{

	if(alpha < 0.001)
	{
		return Vector3(0,0,0);
	}

    Vector3 L = Vector3(0.0f, 0.0f, 0.0f);
    
    const Vector3 viewDir = -ray.d; // d is a unit vector
    
    const Lights *lightlist = scene.lights();
    
    // loop over all of the lights
    Lights::const_iterator lightIter;
    for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
    {
        Light* pLight = *lightIter;
    
        Vector3 l = pLight->getPosition() - hit.P;
        
        // the inverse-squared falloff
        float falloff = l.length2();
        
        // normalize the light direction
        l /= sqrt(falloff);

        // get the diffuse component
        float nDotL = dot(hit.N, l);
        Vector3 result = pLight->color();
		result *= getDiffuseColor();
        
        L += std::max(0.0f, nDotL/falloff * pLight->power() / PI) * result;
    }
    
    // add the ambient component
    L += m_ka;
    
    return L*alpha;
}
