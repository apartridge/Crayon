#pragma once

#include "Matrix4x4.h"
#include "BoundingBox.h"
#include <vector>

struct TriangleMaterialName
{
	Material* material;
	const char* name;
};

class TriangleMesh
{
public:
    TriangleMesh(Material* default_material = NULL);
    TriangleMesh(Vector3*, Vector3*, size_t numTriangles);
    ~TriangleMesh();

	BoundingBox getBoundingBox();
	
    // load from an OBJ file
    bool load(char* file, const Matrix4x4& ctm = Matrix4x4());
    
    // Building mesh programmatically
    Triangle* addTriangle(unsigned int a, unsigned int b, unsigned int c,
        unsigned int na, unsigned int nb, unsigned int nc);

    Triangle* addTriangle(unsigned int a, unsigned int b, unsigned int c, unsigned int normal);

    // For single triangles
    void createSingleTriangle();
    inline void setV1(const Vector3& v) {m_vertices[0] = v;}
    inline void setV2(const Vector3& v) {m_vertices[1] = v;}
    inline void setV3(const Vector3& v) {m_vertices[2] = v;}
    inline void setN1(const Vector3& n) {m_normals[0] = n;}
    inline void setN2(const Vector3& n) {m_normals[1] = n;}
    inline void setN3(const Vector3& n) {m_normals[2] = n;}

    struct TupleI3
    {
        unsigned int x, y, z;
    };

    struct VectorR2
    {
        float x, y;
    };

    Vector3* vertices()     {return m_vertices;}
    Vector3* normals()      {return m_normals;}
	VectorR2* texCoords()	{return m_texCoords;}

    TupleI3* vIndices()     {return m_vertexIndices;}
    TupleI3* nIndices()     {return m_normalIndices;}
	TupleI3* texCoordIndices() {return m_texCoordIndices;}


	Material** materials()  {return m_materials;}
    int numTris()           {return m_numTris;}
	
	void connectNameToMaterial(const char* name, Material* material);
	void setDefaultMaterial(Material* m)
	{
		m_defaultMaterial = m;
	}	
	
	Material* findMaterialByName(const char* const name);

	void addMeshToScene(Scene* scene);

	int numVertices()
	{
		return m_numVertices;
	}


protected:
    void loadObj(FILE* fp, const Matrix4x4& ctm);
	void calculateBoundingBox();

    Vector3* m_normals;
    Vector3* m_vertices;
    VectorR2* m_texCoords;
	Material** m_materials;

    TupleI3* m_normalIndices;
    TupleI3* m_vertexIndices;
    TupleI3* m_texCoordIndices;
    unsigned int m_numTris;
	unsigned int m_numVertices;
	unsigned int m_numMaterials;

	bool m_hasBoundingBox;
	BoundingBox m_boundingBox;


	Material* m_defaultMaterial;
	std::vector<Material*> m_materialList;
	std::vector<const char*> m_materialNames;

	


};
