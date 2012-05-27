#include "geometry/Sphere.h"
#include "geometry/Ray.h"
#include "sysutils/Console.h"
#include "../../headers\renderer\glut.h"


Sphere::Sphere()
{
}

Sphere::~Sphere()
{
}

void Sphere::renderGL()
{
	Vector3 materialColor = Vector3(0.5);//this->m_material->getDiffuseColor();
	glColor4f(materialColor.x, materialColor.y, materialColor.z, 1.0);//this->m_material->getOpacity());
    glTranslatef(m_center.x, m_center.y, m_center.z);
    glutSolidSphere(m_radius, 20, 20);
	glTranslatef(-m_center.x, -m_center.y, -m_center.z);
}


void Sphere::calculateBoundingBox()
{
	m_boundingBox.min = Vector3(  m_center.x - m_radius,  m_center.y - m_radius, m_center.z - m_radius  );
	m_boundingBox.max = Vector3(  m_center.x + m_radius,  m_center.y + m_radius, m_center.z + m_radius  );
}

bool Sphere::intersect(HitInfo& result, Ray& ray, float tMin, float tMax)
{
    const Vector3 toO = ray.o - m_center; 

    const float a = ray.d.length2(); 
    const float b = dot(2*ray.d, toO);
    const float c = toO.length2() - m_radius*m_radius;

    const float discrim = b*b-4.0f*a*c; 

    if (discrim < 0) 
        return false;   // quadratic equation would yield imaginary numbers

    const float sqrt_discrim = sqrt(discrim); 

    // solve the quadratic equation
    const float t[2] = {(-b-sqrt_discrim)/(2.0f*a), (-b+sqrt_discrim)/(2.0f*a)}; 

    // since we know that discrim >= 0, t[0] < t{1]
    // return the t closest to us that is within range
    if ((t[0] > tMin) && (t[0] < tMax))
    {
        result.t = t[0];
    }
    else if((t[1] > tMin) && (t[1] < tMax))
    {
        result.t = t[1];
    }
    else
    {
        // neither of the solutions are in the required range
        return false; 
    }

    result.P = ray.o + result.t*ray.d; 
    result.N = (result.P-m_center); 
    result.N.normalize(); 
    result.material = this->m_material; 

    return true;
}
