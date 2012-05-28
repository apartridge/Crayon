#include "geometry/TriangleMesh.h"
#include "geometry/Triangle.h"
#include "geometry/Scene.h"
#include "sysutils/Console.h"
#include <cstring>

#ifdef WIN32
// disable useless warnings
#pragma warning(disable:4996)
#endif

TriangleMesh::TriangleMesh(Material* default_material) :
    m_normals(0),
    m_vertices(0),
    m_texCoords(0),
    m_normalIndices(0),
    m_vertexIndices(0),
    m_texCoordIndices(0),
	m_hasBoundingBox(false),
	m_numVertices(0),
	m_numMaterials(0),
	m_defaultMaterial(NULL)
{

}

TriangleMesh::~TriangleMesh()
{
    delete [] m_normals;
    delete [] m_vertices;
    delete [] m_texCoords;
    delete [] m_normalIndices;
    delete [] m_vertexIndices;
    delete [] m_texCoordIndices;
}

/*
 * Allocate space to n triangles, with predefined lists of normals and vertices
 */
TriangleMesh::TriangleMesh(Vector3 *vertices, Vector3 *normals, size_t numTriangles)
{
    m_numTris = 0;
    m_vertices = vertices;
    m_normals = normals;

    m_vertexIndices = new TupleI3[numTriangles];
    m_normalIndices = new TupleI3[numTriangles];
}

Triangle* TriangleMesh::addTriangle(unsigned int a, unsigned int b, unsigned int c,
    unsigned int na, unsigned int nb, unsigned int nc)
{
    Triangle *t = new Triangle(this, m_numTris);

    m_vertexIndices[m_numTris].x = a;
    m_vertexIndices[m_numTris].y = b;
    m_vertexIndices[m_numTris].z = c;

    m_normalIndices[m_numTris].x = na;
    m_normalIndices[m_numTris].y = nb;
    m_normalIndices[m_numTris].z = nc;

    m_numTris++;
    return t;
}

Triangle* TriangleMesh::addTriangle(unsigned int a, unsigned int b, unsigned int c,
    unsigned int normal)
{
    return addTriangle(a, b, c, normal, normal, normal);
}

void TriangleMesh::createSingleTriangle()
{
    m_normals = new Vector3[3];
    m_vertices = new Vector3[3];
    m_texCoords = new VectorR2[3];

    m_texCoords[0].x = 0.0f;
    m_texCoords[0].y = 0.0f;
    m_texCoords[1].x = 1.0f;
    m_texCoords[1].y = 0.0f;
    m_texCoords[2].x = 0.0f;
    m_texCoords[2].y = 1.0f;

    m_normalIndices = new TupleI3[1];
    m_vertexIndices = new TupleI3[1];
    m_texCoordIndices = new TupleI3[1];

    m_vertexIndices[0].x = 0;
    m_vertexIndices[0].y = 1;
    m_vertexIndices[0].z = 2;

    m_normalIndices[0].x = 0;
    m_normalIndices[0].y = 1;
    m_normalIndices[0].z = 2;

    m_texCoordIndices[0].x = 0;
    m_texCoordIndices[0].y = 1;
    m_texCoordIndices[0].z = 2;

    m_numTris = 1;
	m_numVertices = 3;
}

void TriangleMesh::connectNameToMaterial(const char* name, Material* material)
{
	m_materialList.push_back(material);
	m_materialNames.push_back(name);
}


//************************************************************************
// You probably don't want to modify the following functions
// They are for loading .obj files
//************************************************************************

bool TriangleMesh::load(char* file, const Matrix4x4& ctm)
{
    FILE *fp = fopen(file, "rb");
    if (!fp)
    {
        error("Cannot open \"%s\" for reading.\n",file);
        return false;
		exit(1);
    }

    loadObj(fp, ctm);
    printf("Loaded \"%s\" with %d tri's, %d vertices and %d materials.\n",
			file, m_numTris, m_numVertices, m_numMaterials);
    fclose(fp);

    return true;
}

void getIndices(char *word, int *vindex, int *tindex, int *nindex)
{
    char *null = " ";
    char *ptr;
    char *tp;
    char *np;

    // by default, the texture and normal pointers are set to the null string
    tp = null;
    np = null;

    // replace slashes with null characters and cause tp and np to point
    // to character immediately following the first or second slash
    for (ptr = word; *ptr != '\0'; ptr++)
    {
        if (*ptr == '/')
        {
            if (tp == null)
                tp = ptr + 1;
            else
                np = ptr + 1;

            *ptr = '\0';
        }
    }

    *vindex = atoi (word);
    *tindex = atoi (tp);
    *nindex = atoi (np);
}


void TriangleMesh::loadObj(FILE* fp, const Matrix4x4& ctm)
{
    int nv=0, nt=0, nn=0, nf=0;
    char line[81];
    while (fgets(line, 80, fp) != 0)
    {
        if (line[0] == 'v')
        {
            if (line[1] == 'n')
                nn++;
            else if (line[1] == 't')
                nt++;
            else
                nv++;
        }
        else if (line[0] == 'f')
        {
            nf++;
        }
    }
    fseek(fp, 0, 0);


    m_normals = new Vector3[std::max(nv,nf)];
    m_vertices = new Vector3[nv];
	m_materials = new Material*[nf];

	Material* currentMaterial = m_defaultMaterial;

    if (nt)
    {   // got texture coordinates
        m_texCoords = new VectorR2[nt];
        m_texCoordIndices = new TupleI3[nf];
    }

    m_normalIndices = new TupleI3[nf]; // always make normals
    m_vertexIndices = new TupleI3[nf]; // always have vertices

    m_numTris = 0;
    int nvertices = 0;
    int nnormals = 0;
    int ntextures = 0;

    Matrix4x4 nctm = ctm;
    nctm.invert();
    nctm.transpose();
    nctm.invert();

    while (fgets(line, 80, fp) != 0)
    {
        if (line[0] == 'v')
        {
            if (line[1] == 'n')
            {
                float x, y, z;
                sscanf(&line[2], "%f %f %f\n", &x, &y, &z);
                Vector3 n(x, y, z);
                m_normals[nnormals] = nctm*n;
                m_normals[nnormals].normalize();
                nnormals++;
            }
            else if (line[1] == 't')
            {
                float x, y;
                sscanf(&line[2], "%f %f\n", &x, &y);
                m_texCoords[ntextures].x = x;
                m_texCoords[ntextures].y = y;
                ntextures++;
            }
            else
            {
                float x, y, z;
                sscanf(&line[1], "%f %f %f\n", &x, &y, &z);
                Vector3 v(x, y, z);
                m_vertices[nvertices] = ctm*v;
                nvertices++;
            }
        }
        else if (line[0] == 'f')
        {
            char s1[32], s2[32], s3[32];
            int v, t, n;
            sscanf(&line[1], "%s %s %s\n", s1, s2, s3);

            getIndices(s1, &v, &t, &n);
            m_vertexIndices[m_numTris].x = v-1;
            if (n)
                m_normalIndices[m_numTris].x = n-1;
            if (t)
                m_texCoordIndices[m_numTris].x = t-1;
            getIndices(s2, &v, &t, &n);
            m_vertexIndices[m_numTris].y = v-1;
            if (n)
                m_normalIndices[m_numTris].y = n-1;
            if (t)
                m_texCoordIndices[m_numTris].y = t-1;
            getIndices(s3, &v, &t, &n);
            m_vertexIndices[m_numTris].z = v-1;
            if (n)
                m_normalIndices[m_numTris].z = n-1;
            if (t)
                m_texCoordIndices[m_numTris].z = t-1;

            if (!n)
            {   // if no normal was supplied
                Vector3 e1 = m_vertices[m_vertexIndices[m_numTris].y] -
                             m_vertices[m_vertexIndices[m_numTris].x];
                Vector3 e2 = m_vertices[m_vertexIndices[m_numTris].z] -
                             m_vertices[m_vertexIndices[m_numTris].x];

                m_normals[nn] = cross(e1, e2);
                m_normalIndices[nn].x = nn;
                m_normalIndices[nn].y = nn;
                m_normalIndices[nn].z = nn;
                nn++;
            }

			if(currentMaterial == NULL)
			{
				error("Missing a material for this object. Set default material.\n");
				exit(1);
			}

			m_materials[m_numTris] = currentMaterial;

            m_numTris++;
        } 
		// Update the current material
		else if(line[0] == 'u' && line[1] == 's' && line[2] == 'e' && line[3] == 'm' && line[4] == 't' && line[5] == 'l')
		{
			char materialName[100];
			sscanf(&line[6], "%s\n", materialName);
			if(strlen(materialName) > 0)
			{
				printf("Contains material \"%s\".\n", materialName);
				currentMaterial = findMaterialByName(materialName);
				m_numMaterials++;
			}
		}
    }

	m_numVertices = nvertices;
}

Material* TriangleMesh::findMaterialByName(const char* const name)
{
	for(int i = 0; i < m_materialNames.size(); i++)
	{
		if(strcmp(name, m_materialNames.at(i)) == 0)
		{
			return m_materialList.at(i);
		}
	}
	return m_defaultMaterial;
}
