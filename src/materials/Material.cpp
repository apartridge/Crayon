#include "../../headers/materials/Material.h"

Material::Material(Vector3 diffuseColor, Medium med, float opacity){
	m_diffuseColor = diffuseColor;	
	m_medium = med;
	m_opacity = opacity;
}

Material::~Material()
{
}

Vector3 Material::shade(const Ray&, const HitInfo&, const Scene&, float alpha, const int depth) const
{
    throw "Not Implemented!";
}

const Vector3 & Material::getDiffuseColor() const
{
	return m_diffuseColor;
}