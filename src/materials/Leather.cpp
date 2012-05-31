#include "materials/Leather.h"
#include "materials/Worley.h"
#include "sysutils/Random.h"
#include <cassert>

Leather::Leather() : Lambert(Vector3(1,0,0))
{
}

Leather::~Leather()
{
}

float min(float a, float b)
{
	if(a < b) return a; else return b;
}

Vector3 Leather::shadeLight(const Light& light, const Ray& ray, const HitInfo& hit, const Scene& scene, const int depth) const
{


	const Vector3 C (184.0 / 255.0, 184.0 / 255.0, 184.0 / 255.0);
   
   const float m = 0.05;
   const float RI = 0.1;
  
   Vector3 L = light.getPosition() - hit.P;
   L.normalize();

   Vector3 V = ray.direction();
   V.normalize();
   Vector3 H = (L + V).normalized();
   Vector3 N = hit.N;
   N.normalize();
   
   float NH = (dot(N,H));
   float VH = (dot(V,H));
   float NV = dot(N,V);
   float NL = (dot(L,N));
   
   // Beckmann's distribution function
   float NH2 = NH * NH;
   float m2 = m * m;
   float D = (1.0 / m2 * NH2 * NH2) * (exp(-((1.0 - NH2) / (m2 * NH2))));

   // Fresnel
   float F = RI + (1 - RI) * powf((1.0 - NV), 5.0f);   
   
   // geometric term
   float G = min(1.0, min((2.0 * NH * NL) / VH, (2 * NH * NV) / VH));
   float S = (F * D * G) / (PI * NL * NV);

   const float dw = 0.2;
   const float Si = 0.1;
   const float Di = 0.1;
   
   return (NL * (dw * ((Di * NL * C) + (Si * S))));
























   /*

	Vector3 skinColor = Vector3(68, 26, 23)/255.0;

	const float scale = 10;


	Vector3 normal = hit.N;

	
	// Color
	
    Vector3 L = skinColor;
    Vector3 l = light.getPosition() - hit.P;

    // Calculate shadow ray
    HitInfo shadowHit;
    Ray shadowRay (hit.P, l.normalized());
    
	float visibility = light.visibility(hit.P, scene);
	
	float falloff = l.length2();
	l /= sqrt(falloff);

	float F[4];
	
	WorleyNoise::turbulence(F, 0.9, scale*hit.P.x, scale*hit.P.y, scale*hit.P.z);
    float fine_grid = 0.1*(F[1] - F[0]);
	
	if(fine_grid > 1)
	{
		fine_grid = 1;
	}

	if(fine_grid < 0)
	{
		fine_grid = 0;
	}

	fine_grid = pow(fine_grid + 0.980, 3);
	if(fine_grid > 1)
	{
		fine_grid = 1;
	}
	fine_grid = 1.0 - fine_grid;



    float nDotL = dot(normal, l);
	//return normal;

	//Vector3 wr = reflect(ray, hit);
	//float shininess = pow(dot(wr, ray.direction()),20);
	//return Vector3(fine_grid);

	float m_glossFactor = 0.01;
	float m_glossPower = 10;

	Vector3 halfway = l - ray.direction();
	halfway.normalize();
	Vector3 specular = 0.1*m_glossFactor*pow(dot(normal, halfway), m_glossPower)*(m_glossPower+1)/(2*PI)*Vector3(1,0,0);

    return (L - fine_grid)*nDotL + specular;*/
}