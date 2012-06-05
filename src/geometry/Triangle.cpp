#include "geometry/Triangle.h"
#include "geometry/TriangleMesh.h"
#include "geometry/Ray.h"
#include "../../headers\renderer\glut.h"

Triangle::Triangle(TriangleMesh * m, unsigned int i, Vector3 gNormal) :
    m_mesh(m), m_index(i), m_gNormal(gNormal)
{

}

void cube(BoundingBox& box)
{
	const Vector3& min = box.getMin();
	const Vector3& max = box.getMax();
	void* a = (void*)&max;
	glBegin(GL_QUADS);
	const float padding = 0.0;
	// side min.z
	glVertex3f(min.x-padding, min.y-padding, min.z-padding);
	glVertex3f(min.x-padding, max.y-padding, min.z-padding);
	glVertex3f(max.x+padding, max.y+padding, min.z-padding);
	glVertex3f(max.x+padding, min.y-padding, min.z-padding);
	// side max.z
	glVertex3f(min.x-padding, min.y-padding, max.z+padding);
	glVertex3f(min.x-padding, max.y+padding, max.z+padding);
	glVertex3f(max.x+padding, max.y+padding, max.z+padding);
	glVertex3f(max.x+padding, min.y-padding, max.z+padding);
	// Side max.x
	glVertex3f(max.x+padding, min.y-padding, min.z-padding);
	glVertex3f(max.x+padding, max.y+padding, min.z-padding);
	glVertex3f(max.x+padding, max.y+padding, max.z+padding);
	glVertex3f(max.x+padding, min.y-padding, max.z+padding);
	// Side min.x
	glVertex3f(min.x-padding, min.y-padding, min.z-padding);
	glVertex3f(min.x-padding, max.y+padding, min.z-padding);
	glVertex3f(min.x-padding, max.y+padding, max.z+padding);
	glVertex3f(min.x-padding, min.y-padding, max.z+padding);
	// Side max.y
	glVertex3f(min.x-padding, max.y+padding, min.z-padding);
	glVertex3f(max.x+padding, max.y+padding, min.z-padding);
	glVertex3f(max.x+padding, max.y+padding, max.z+padding);
	glVertex3f(min.x-padding, max.y+padding, max.z+padding);
	// Side min.y
	glVertex3f(min.x-padding, min.y-padding, min.z-padding);
	glVertex3f(max.x+padding, min.y-padding, min.z-padding);
	glVertex3f(max.x+padding, min.y-padding, max.z+padding);
	glVertex3f(min.x-padding, min.y-padding, max.z+padding);
	glEnd();

}

Triangle::~Triangle()
{

}

inline float min(float a, float b, float c){
	float min = a;

	if(b < min)
	{
		min = b;
	}
	if(c < min)
	{
		min = c;
	}

	return min;
}

inline float max(float a, float b, float c){
	float max = a;
	if(b > a)
	{
		max = b;
	}
	if(c > max)
	{
		max = c;
	}
	return max;
}

void Triangle::calculateBoundingBox()
{
	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
    const Vector3 & v0 = m_mesh->vertices()[ti3.x];
    const Vector3 & v1 = m_mesh->vertices()[ti3.y];
    const Vector3 & v2 = m_mesh->vertices()[ti3.z];

	float minx, miny, minz, maxx, maxy, maxz;
	float padd = 0.00001;
	
	minx = min(v0.x, v1.x, v2.x) - padd;
	maxx = max(v0.x, v1.x, v2.x) + padd;

	miny = min(v0.y, v1.y, v2.y) - padd;
	maxy = max(v0.y, v1.y, v2.y) + padd;

	minz = min(v0.z, v1.z, v2.z) - padd;
	maxz = max(v0.z, v1.z, v2.z) + padd;

	m_boundingBox = BoundingBox(Vector3(minx, miny, minz),
		                        Vector3(maxx, maxy, maxz));

}

void Triangle::renderGL()
{
    TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
    const Vector3 & v0 = m_mesh->vertices()[ti3.x]; //vertex a of triangle
    const Vector3 & v1 = m_mesh->vertices()[ti3.y]; //vertex b of triangle
    const Vector3 & v2 = m_mesh->vertices()[ti3.z]; //vertex c of triangle
	
	Vector3 materialColor = Vector3(0.5); //this->m_material->getDiffuseColor();
	glColor3f(materialColor.x, materialColor.y, materialColor.z);

    glBegin(GL_TRIANGLES);
    glVertex3f(v0.x, v0.y, v0.z);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glEnd();

	//cube(m_boundingBox);
}

bool Triangle::intersect(HitInfo& result, Ray& r, float tMin, float tMax)
{
	#if RENDERING_STATS
	rendering_statistics->increment_triangle_intsects(1);
	#endif

	TriangleMesh::TupleI3 ti3 = m_mesh->vIndices()[m_index];
    const Vector3& A = m_mesh->vertices()[ti3.x];
    const Vector3& B = m_mesh->vertices()[ti3.y];
    const Vector3& C = m_mesh->vertices()[ti3.z];

	// Stores geometric normal as part of the struct.
	if(this->m_gNormal == NULL)
	{
		this->m_gNormal = cross(B-A, C-A);
	}

	double detA = -dot(this->m_gNormal, r.direction());

	if(detA == 0)
	{
		return false;
	}

	double t = dot(this->m_gNormal, r.origin()-C)/detA;

	if(t >= tMin && t <= tMax)
	{
		Vector3 roMinA = r.origin()-A;
		double alpha = dot(cross(roMinA, C-A), -r.direction())/detA;
		double beta = dot(cross(B-A, roMinA), -r.direction())/detA;

		if(alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && alpha + beta <= 1)
		{

			TriangleMesh::TupleI3 tn3 = m_mesh->nIndices()[m_index];
			const Vector3& N1 = m_mesh->normals()[tn3.x];
			const Vector3& N2 = m_mesh->normals()[tn3.y];
			const Vector3& N3 = m_mesh->normals()[tn3.z];

			result.N = (1-alpha-beta)*N1 + alpha*N2 + beta*N3;
			result.N.normalize();

			result.P = r.origin() + t*r.direction();
			result.t = t;
			result.material = m_material;

			// Texture Coordinates

			if(m_mesh->hasTexCoords())
			{
				TriangleMesh::TupleI3 texcoords3 = m_mesh->texCoordIndices()[m_index];
				if(texcoords3.x >= 0)
				{
					const TriangleMesh::VectorR2 uv1 = m_mesh->texCoords()[texcoords3.x];
					const TriangleMesh::VectorR2 uv2 = m_mesh->texCoords()[texcoords3.y];
					const TriangleMesh::VectorR2 uv3 = m_mesh->texCoords()[texcoords3.z];

					result.textureU = (1-alpha-beta)*uv1.x + alpha*uv2.x + beta*uv3.x;
					result.textureV = (1-alpha-beta)*uv1.y + alpha*uv2.y + beta*uv3.y;
				}
			}

			return true;
		}
	}

    return false;
}
