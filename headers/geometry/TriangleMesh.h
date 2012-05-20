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
    ~TriangleMesh();

	BoundingBox getBoundingBox();
	

    // load from an OBJ file
    bool load(char* file, const Matrix4x4& ctm = Matrix4x4());

    // for single triangles
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
    TupleI3* vIndices()     {return m_vertexIndices;}
    TupleI3* nIndices()     {return m_normalIndices;}
    int numTris()           {return m_numTris;}
	

	void connectMaterialToName(Material* a, const char* name);
		


	int numVertices()
	{
		return m_numVertices;
	}

	void addMeshToScene(Scene* scene);


protected:
    void loadObj(FILE* fp, const Matrix4x4& ctm);
	void calculateBoundingBox();


    Vector3* m_normals;
    Vector3* m_vertices;
    VectorR2* m_texCoords;

    TupleI3* m_normalIndices;
    TupleI3* m_vertexIndices;
    TupleI3* m_texCoordIndices;
    unsigned int m_numTris;
	unsigned int m_numVertices;

	bool m_hasBoundingBox;
	BoundingBox m_boundingBox;

	// Materials
	/*Material* m_defaultMaterial;
	std::vector<TriangleMaterial> m_materialNames;*/
};

typedef std::vector<TriangleMesh*> TriangleMeshes;