#include "../../headers/materials/Stone.h"

#include <cassert>

const float SCALE =0.5;//0.33f;
const float FINE_SCALE = 1.0f;
const float GAIN = 1.0f;

Stone::Stone() : Material(Vector3(1,1,1), Medium(1), 1)
{
}

Stone::~Stone()
{
}

float stones(float x, float y, float z, float F[4], unsigned long *id)
{
	float at[3] = { x*SCALE, y*SCALE, z*SCALE };
	float delta[4][3];
    WorleyNoise::noise3D(at, 4, F, delta, id);
    return (0.95 - 0.9/(1+pow(F[1] - F[0], 1.1f))); 
}

Vector3 Stone::shade(const Ray& ray, const HitInfo& hit, const Scene& scene, float alpha, const int depth) const
{
	if(alpha < 0.001)
	{
		return Vector3(0,0,0);
	}


    Vector3 color[] = {
        2*Vector3(0x33, 0x38, 0x37),
        Vector3(0x63, 0x6a, 0x6c),
        Vector3(0xa3, 0x9b, 0x91),
        0.7*Vector3(0xff, 0xff, 0xff),
        Vector3(0xcb, 0xaf, 0x92)
    };

    // Separate each stone, find id
	unsigned long *id = new unsigned long;
    float Fcoarse[4];
    float stone_grid = stones(hit.P.x, hit.P.y, hit.P.z, Fcoarse, id);
    
    // Additional fractal noise
    float F[4];
    WorleyNoise::turbulence(F, 2, SCALE*hit.P.x, SCALE*hit.P.y, SCALE*hit.P.z);
    float fine_grid = 1 - 0.7/(1 + sqrt(F[1] - F[0]));
    
    // Add perlin inside each for fine detail 
    float perlin_turb = PerlinNoise::turbulence(4, hit.P.x*FINE_SCALE, hit.P.y*FINE_SCALE, hit.P.z*FINE_SCALE);
    float stone_detail = (1/(1+sqrt(Fcoarse[0]))) * std::abs(perlin_turb + 1) / 2.0f;
    
    Vector3 & pixel_color = color[*id % 5] / 255;
    pixel_color *= stone_detail * stone_grid * fine_grid;

    // Calculate illumination
    Vector3 L = Vector3(0, 0, 0);
    const Lights *lightlist = scene.lights();
    Lights::const_iterator lightIter;
    for (lightIter = lightlist->begin(); lightIter != lightlist->end(); lightIter++)
    {
        Light* pLight = *lightIter;
    
        Vector3 l = pLight->getPosition() - hit.P;

        // Calculate shadow ray
        HitInfo shadowHit;
        Ray shadowRay;
        shadowRay.d = l.normalized(); 
        shadowRay.o = hit.P;

        if (!scene.trace(shadowHit, shadowRay, epsilon)) 
        {
            // the inverse-squared falloff
            float falloff = l.length2();
        
            // normalize the light direction
            l /= sqrt(falloff);

            // get the diffuse component
            float nDotL = dot(hit.N, l);
            Vector3 result = pLight->color();
        
            L += pixel_color * std::max(0.0f, nDotL/falloff * pLight->power() / PI) * result;
        }
    }
    return alpha*Vector3(pow(L[0] * GAIN, (1/2.2f)),
						   pow(L[1] * GAIN, (1/2.2f)),
						   pow(L[2] * GAIN, (1/2.2f)));
}