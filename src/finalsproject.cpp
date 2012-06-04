#include "finalsproject.h"
#include "materials/Lambert.h"
#include "materials/LightSource.h"
#include "lights/SquareLight.h"

#include "materials/Glass.h"
#include "materials/Wood.h"
#include "materials/Texture.h"
#include "materials/LambertTwoColor.h"
#include "materials/Skydome.h"

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
	//const float SCALE_FROM_MATERIALS_TO_FINALSCENE = 1.1;

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
	g_camera->setFocalLength(0); // 6.5
	g_camera->setAperture(0.07);

    // Add HDR sphere map
    Sphere *hdrSphere = new Sphere();
    hdrSphere->setCenter(Vector3(0));
    hdrSphere->setRadius(50);
    hdrSphere->setMaterial(new Skydome("rnl_probe.pfm"));
    g_scene->addObject(hdrSphere);
    

    Material* materialOne = new Lambert(Vector3(0.5));
	TriangleMesh * mesh = new TriangleMesh;
	mesh->setDefaultMaterial(materialOne);



    // Area light
    if (false)
    {
        const float w_dist = 5;
        SquareLight* areaLight = new SquareLight(Vector3(-1, 0, 0), // Normal
                                                 Vector3(17.1 + w_dist, 11.0, 3.5), 
                                                 Vector3(17.1 + w_dist, -0.5, 22.15));
        areaLight->setColor(Vector3(1));
        areaLight->setPower(1000);
        g_scene->addLight(areaLight);

    }
    // Point light
    else
    {
        // Visual target
        /*Sphere *trg = new Sphere();
        trg->setCenter(Vector3(15.81, 5.40, 9.3));
        trg->setRadius(8);
        g_scene->addObject(trg);*/

        PointLight * light = new PointLight;
        light->setPosition(Vector3(30, 11, 10));
        light->setColor(Vector3(1, 1, 1));
        light->setPower(4000);

        // Set target at windows, to not waste photons
        LightTarget* target = new LightTarget(Vector3(15.81, 5.40, 9.3), 15);
        light->setTarget(target);

        g_scene->addLight(light);
    }
    

	// Glass on table
	Material* glassWine = new Glass();
	mesh->connectNameToMaterial("GlassWine", glassWine);

	
	/*
	// Outside
	*/
	
    // Windows and area lights
    Material* windowGlass = new Glass();
    mesh->connectNameToMaterial("BigWindows", windowGlass);

    Material* windowEdge = new Lambert(Vector3(0.3,0.3,0.3));
    mesh->connectNameToMaterial("BigWindowEdge", windowEdge);
	Material* windowOutside = new Lambert(Vector3(0,0,0));
	mesh->connectNameToMaterial("WindowOutside", windowOutside);

	// Porch

	Wood* porchWoodenTop = new Wood(Vector3(97, 14, 14)/255.0, Vector3(158, 33, 33)/255.0, 0.3, 10);
	mesh->connectNameToMaterial("PorchTop", porchWoodenTop);

	Wood* porchWood = new Wood(Vector3(70, 40, 12)/255.0, Vector3(112, 60, 9)/255.0, 0.3, 10);
	mesh->connectNameToMaterial("PorchWood", porchWood);


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

	Vector3 floorTrimBase = Vector3(170, 85, 34)/255.0;
	Vector3 floorTrimHigh = Vector3(218, 133, 34)/255.0;
	Material* floorTrimMat = new Wood(floorTrimBase, floorTrimHigh, 2, 12, Vector3(1,1,0), 0.6);

	mesh->connectNameToMaterial("Floor", floor);
	mesh->connectNameToMaterial("FloorTrim", floorTrimMat);

	/*
	// Walls
	*/

	Material* ceiling = new Lambert(Vector3(1.0, 1.0, 1.0f));
	Material* walls = new LambertTwoColor(Vector3(0.99), Vector3(54, 46, 37)*2/255.0 , 0.4, 5.0  );
	mesh->connectNameToMaterial("Walls", walls);
	mesh->connectNameToMaterial("Ceiling", ceiling);

	/*
	// WOODEN TABLE AND CHAIRS
	*/
	
	Vector3 baseColor = Vector3(24, 18, 14)/255.0;
	Vector3 highColor = Vector3(68, 34, 10)/255.0;
	const float scale = 0.54;
	float perlinScale = 20;
	Wood* tableWood = new Wood(baseColor, highColor, scale, perlinScale, Vector3(0.5,0.0,0.5), 2);
	tableWood->setGlossiness(100, 0.005, Vector3(1));
	mesh->connectNameToMaterial("TableWood_FW_WD_BB.JPG", tableWood);  // have to remove this .jpg crap
	mesh->connectNameToMaterial("ChairWood_CHR00401.jpg", tableWood);
	
	
	Material* chairFabric = new Lambert(Vector3(0.8, 0.1, 0.1));
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
	Material* staircaseSides = new Wood(stairCaseBaseColor, stairCaseHighColor, 0.53, 12, Vector3(0,1,0), 1.36);
	mesh->connectNameToMaterial("StairsSides", staircaseSides);

	// Dark Steps

	Vector3 staircaseDarkStepBase = Vector3(45, 30, 18)/255.0;
	Vector3 staircaseDarkStepHigh = Vector3(123, 82, 49)/255.0;

	const float staircaseDarkStepScale = 1.5;
	float staircaseDarkStepPerlinScale = 10;
	Material* materialStairDarkSteps = new Wood(staircaseDarkStepBase, staircaseDarkStepHigh,
												staircaseDarkStepScale,
												staircaseDarkStepPerlinScale);

	mesh->connectNameToMaterial("StairsSteps", materialStairDarkSteps);
	mesh->connectNameToMaterial("StairsHandguards", materialStairDarkSteps);

	/*
	// SOFA AND TABLE
	*/
	Material* sofaFabric = new Lambert(Vector3(0.7,0.1,0.1));
	mesh->connectNameToMaterial("SofaFabric", sofaFabric);

	Material* sofaFeet = new Lambert(Vector3(0.8,0.8,0.1));
	mesh->connectNameToMaterial("SofaFeet", sofaFeet);

	//Material* frostedGlassTable = new FrostedGlass(Vector3(0.2), Vector3(0.9));
    mesh->connectNameToMaterial("TableTopGlass", new Glass());




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
