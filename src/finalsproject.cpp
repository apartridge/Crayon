#include "finalsproject.h"
#include "materials/Lambert.h"
#include "materials/LightSource.h"
#include "lights/SquareLight.h"

#include "materials/Glass.h"
#include "materials/Wood.h"
#include "materials/Texture.h"
#include "materials/LambertTwoColor.h"

// local helper function declarations
namespace
{
	//void addMeshTrianglesToScene(TriangleMesh * mesh, Material * material);
	/*inline Matrix4x4 translate(float x, float y, float z);
	inline Matrix4x4 scale(float x, float y, float z);
	inline Matrix4x4 rotate(float angle, float x, float y, float z);*/
}

void makeFinalScene() 
{
	// This factor adjusts the Noise scale from the Materials.cpp test scenes to
	// world.obj
	const float SCALE_FROM_MATERIALS_TO_FINALSCENE = 1.1;

    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;
    g_image->resize(512, 512);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(-9, 5, -9));
    g_camera->setLookAt(Vector3(-6, 3, -6));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // Create light sources

    PointLight * light = new PointLight;
    light->setPosition(Vector3(4, 6, 4));
    light->setColor(Vector3(1, 1, 1));
    light->setPower(35);
    g_scene->addLight(light);

	PointLight * light2 = new PointLight;
    light2->setPosition(Vector3(-7, 6, -7));
    light2->setColor(Vector3(1, 1, 1));
    light2->setPower(35);
    g_scene->addLight(light2);


    Material* materialOne = new Lambert(0.5*Vector3(1.0f, 1.0f, 1.0f));
    
	TriangleMesh * mesh = new TriangleMesh;
	mesh->setDefaultMaterial(materialOne);

    

	// Glass on table
	Material* glassWine = new Glass();
	mesh->connectNameToMaterial("GlassWine", glassWine);

	


	/*
	// Floor
	*/

	Texture* floor = new Texture();
	TextureImage* diffuse = TextureImage::loadFromPFM("materials/floor.pfm");
	TextureImage* normal = TextureImage::loadFromPFM("materials/floorNM.pfm");
	floor->setDiffuse(diffuse);
	Matrix4x4 objWorld;
	objWorld.setIdentity();
	floor->setNormalMap(normal, objWorld, 1);
	mesh->connectNameToMaterial("Floor", floor);

	/*
	// Walls
	*/

	Material* ceiling = new Lambert(Vector3(1.0, 1.0, 1.0f));
	Material* walls = new LambertTwoColor(Vector3(0.99), Vector3(54, 46, 37)/255.0 , 0.4, 5.0  );
	mesh->connectNameToMaterial("Walls", walls);
	mesh->connectNameToMaterial("Ceiling", ceiling);

	/*
	// WOODEN TABLE AND CHAIRS
	*/
		
	Vector3 baseColor = Vector3(24, 18, 14)/255.0;
	Vector3 highColor = Vector3(68, 34, 10)/255.0;
	const float scale = 0.54;
	float perlinScale = 20;
	Wood* tableWood = new Wood(baseColor, highColor, scale, perlinScale);
	tableWood->setGlossiness(500, 0.005, Vector3(1));
	mesh->connectNameToMaterial("TableWood_FW_WD_BB.JPG", tableWood);  // have to remove this .jpg crap
	mesh->connectNameToMaterial("ChairWood_CHR00401.jpg", tableWood);
	
	
	Material* chairFabric = new Lambert(Vector3(0.8, 0, 0));
	mesh->connectNameToMaterial("ChairFabric_CHR00402.jpg", chairFabric);  // have to remove this .jpg



	/*
	// Chess Board
	*/
	Material* white = new Lambert(Vector3(1));
	Material* black = new Lambert(Vector3(0));
	mesh->connectNameToMaterial("ChessWhite", white);
	mesh->connectNameToMaterial("ChessBlack", black);

    Material* glassChessPieceWhite = new Glass();
    Material* glassChessPieceBlack = new Glass();
    mesh->connectNameToMaterial("ChessPiecesOne", glassChessPieceWhite);
    mesh->connectNameToMaterial("ChessPiecesTwo", glassChessPieceBlack);

	Wood* darkWood = new Wood(Vector3(45, 30, 18)/255.0, Vector3(123, 82, 49)/255.0, 0.3, 10);
	darkWood->setGlossiness(300, 0.03, Vector3(1));
    mesh->connectNameToMaterial("ChessBoard", darkWood);

	/*
	// STAIRS
	*/

	// Light Sides

	Vector3 stairCaseBaseColor = Vector3(170, 85, 34)/255.0;
	Vector3 stairCaseHighColor = Vector3(218, 133, 34)/255.0;
	Material* staircaseSides = new Wood(stairCaseBaseColor, stairCaseHighColor, SCALE_FROM_MATERIALS_TO_FINALSCENE*0.53, 12);
	mesh->connectNameToMaterial("StairsSides", staircaseSides);

	// Dark Steps

	Vector3 staircaseDarkStepBase = Vector3(45, 30, 18)/255.0;
	Vector3 staircaseDarkStepHigh = Vector3(123, 82, 49)/255.0;

	const float staircaseDarkStepScale = 1.5;
	float staircaseDarkStepPerlinScale = 10;
	Material* materialStairDarkSteps = new Wood(staircaseDarkStepBase, staircaseDarkStepHigh,
												SCALE_FROM_MATERIALS_TO_FINALSCENE*staircaseDarkStepScale,
												staircaseDarkStepPerlinScale);

	mesh->connectNameToMaterial("StairsSteps", materialStairDarkSteps);
	mesh->connectNameToMaterial("StairsHandguards", materialStairDarkSteps);

	/*
	// SOFA AND TABLE
	*/
	Material* sofaFabric = new Lambert(Vector3(1,0,0));
	mesh->connectNameToMaterial("SofaFabric", sofaFabric);

	Material* sofaFeet = new Lambert(Vector3(1,1,0));
	mesh->connectNameToMaterial("SofaFeet", sofaFeet);

    Material* frostedGlassTable = new FrostedGlass(Vector3(0.2), Vector3(1.0));
    mesh->connectNameToMaterial("TableTopGlass", frostedGlassTable);




    mesh->load("world.obj");
	g_scene->addMesh(mesh);


    g_scene->preCalc();
}



// local helper function definitions
namespace
{
	/*void addMeshTrianglesToScene(TriangleMesh * mesh, Material * material)
	{

		for (int i = 0; i < mesh->numTris(); ++i)
		{
			Triangle* t = new Triangle;
			t->setIndex(i);
			t->setMesh(mesh);
			t->setMaterial(material);
			g_scene->addObject(t);
		}
	}*/
	/*
	inline Matrix4x4 translate(float x, float y, float z)
	{
		Matrix4x4 m;
		m.setColumn4(Vector4(x, y, z, 1));
		return m;
	}


	inline Matrix4x4 scale(float x, float y, float z)
	{
		Matrix4x4 m;
		m.m11 = x;
		m.m22 = y;
		m.m33 = z;
		return m;
	}

	// angle is in degrees
	inline Matrix4x4 rotate(float angle, float x, float y, float z)
	{
		float rad = angle*(PI/180.);
    
		float x2 = x*x;
		float y2 = y*y;
		float z2 = z*z;
		float c = cos(rad);
		float cinv = 1-c;
		float s = sin(rad);
		float xy = x*y;
		float xz = x*z;
		float yz = y*z;
		float xs = x*s;
		float ys = y*s;
		float zs = z*s;
		float xzcinv = xz*cinv;
		float xycinv = xy*cinv;
		float yzcinv = yz*cinv;
    
		Matrix4x4 m;
		m.set(x2 + c*(1-x2), xy*cinv+zs, xzcinv - ys, 0,
			  xycinv - zs, y2 + c*(1-y2), yzcinv + xs, 0,
			  xzcinv + ys, yzcinv - xs, z2 + c*(1-z2), 0,
			  0, 0, 0, 1);
		return m;
	}*/

}
