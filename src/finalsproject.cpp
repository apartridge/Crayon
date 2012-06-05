#include "finalsproject.h"
#include "materials/Lambert.h"
#include "materials/LightSource.h"
#include "lights/SquareLight.h"

#include "materials/Glass.h"
#include "materials/Wood.h"
#include "materials/Texture.h"
#include "materials/LambertTwoColor.h"
#include "materials/Skydome.h"
#include "materials/Fabric.h"
#include "materials/Stone.h"

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
    g_image->resize(1280, 720);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(-5.422, 5.444, -8.215));
    g_camera->setLookAt(Vector3(-3.283, 4.077, -5.123));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);
	g_camera->setFocalLength(3.9);
	g_camera->setAperture(0.017);

    // Add HDR sphere map
    Sphere *hdrSphere = new Sphere();
    hdrSphere->setCenter(Vector3(0));
    hdrSphere->setRadius(200);
    hdrSphere->setMaterial(new Skydome("materials/outside.pfm"));
    g_scene->addObject(hdrSphere);
    

    Material* materialDefault = new Lambert(Vector3(1,0,0));
	TriangleMesh * mesh = new TriangleMesh;
	mesh->setDefaultMaterial(materialDefault);



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
        light->setPosition(Vector3(88, 32, 21)); // Vector3(89.2, 11, 10)*3
        light->setColor(Vector3(1, 1, 1));
        light->setPower(330000); // 4000
        // Set target at windows, to not waste photons
        LightTarget* target = new LightTarget(Vector3(15.81, 5.40, 9.3), 15);
        light->setTarget(target);
        g_scene->addLight(light);
    }
    


	/*
	Debug Light for working on wood
	*/
    
	PointLight * lightInRoom = new PointLight;
    lightInRoom->setPosition(Vector3(-3, 11, -3));
    lightInRoom->setColor(Vector3(1, 1, 1));
    lightInRoom->setPower(200);
    //g_scene->addLight(lightInRoom);

	

	// Glass on table
	Material* glassWine = new Glass();
	mesh->connectNameToMaterial("GlassWine", glassWine);

	
	/*
	// Outside
	*/

	// Windows and area lights
    Material* windowGlass = new Glass(1.5, Vector3(1,1,1));
    mesh->connectNameToMaterial("BigWindows", windowGlass);

    Material* windowEdge = new Lambert(Vector3(0.3,0.3,0.3));
    mesh->connectNameToMaterial("BigWindowEdge", windowEdge);
	Material* windowEdgeInside = new Lambert(Vector3(0.1,0.1,0.1));
    mesh->connectNameToMaterial("BigWindowEdgeInside", windowEdgeInside);
	Material* windowOutside = new Lambert(Vector3(40,31,1)/255.0/4);
	mesh->connectNameToMaterial("WindowOutside", windowOutside);

	// Porch

	Material* porchWoodenTop = new Wood(Vector3(0, 0, 0)/255.0, Vector3(10, 10, 10)/255.0, 0.6);
	mesh->connectNameToMaterial("PorchTop", porchWoodenTop);

	Wood* porchWood =  new Wood(Vector3(20, 8, 5)/255.0, Vector3(22, 10, 11)/255.0, 1, Vector3(1,0,1).normalized(), 0.3);
	mesh->connectNameToMaterial("PorchWood", porchWood);

	Lambert* sunbedFrabric = new Fabric(Vector3(1,1,1));
	mesh->connectNameToMaterial("SunbedFabric", sunbedFrabric);
	mesh->connectNameToMaterial("SunbedWheels", new Lambert(Vector3(0)));


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

	Vector3 floorTrimBase = Vector3(78, 48, 8)/255.0;
	Vector3 floorTrimHigh = floorTrimBase;

	Material* floorTrimMatX = new Wood(floorTrimBase, floorTrimHigh, 0.1, Vector3(0,0,1), 0.00001);
	Material* floorTrimMatY = new Wood(floorTrimBase, floorTrimHigh, 0.1, Vector3(0,0,1), 0.00001);
	// XY in blender  space

	mesh->connectNameToMaterial("Floor", floor);
	mesh->connectNameToMaterial("FloorTrimX", floorTrimMatX);
	mesh->connectNameToMaterial("FloorTrimY", floorTrimMatY);

	/*
	// Walls
	*/

	Material* ceiling = new Lambert(Vector3(0.97));
	Material* walls = new LambertTwoColor(Vector3(0.99), Vector3(54, 46, 37)*2/255.0 , 0.4, 5.0  );
	mesh->connectNameToMaterial("Walls", walls);
	mesh->connectNameToMaterial("Ceiling", ceiling);

	/*
	// WOODEN TABLE AND CHAIRS
	*/
	
	/*Vector3 baseColor = Vector3(31, 22, 9)/255.0;
	Vector3 highColor = Vector3(40, 29, 13)/255.0;
	const float scale = 0.1;
	Wood* tableWood = new Wood(baseColor, highColor, scale, Vector3(0.8,0.0,1).normalized(), 4.5);
	tableWood->setGlossiness(100, 0.005, Vector3(1));
	*/


	Vector3 tableWoodBaseColor = Vector3(69, 22, 1)/255.0;
	Vector3 tableWoodHighColor = tableWoodBaseColor*1.1 + Vector3(5,5,0)/255.0;
	const float scale = 0.70;
	Wood* tableWood = new Wood(tableWoodBaseColor, tableWoodHighColor, scale, Vector3(1,0,0), 3.0f);
    
	Vector3 glossColor = Vector3(1);
	int glossPower = 300;
	float glossFactor = 0.001;
	tableWood->setGlossiness(glossPower, glossFactor, glossColor);
	mesh->connectNameToMaterial("TableWood", tableWood);



	Material* chairFabric = new Fabric(Vector3(0.96, 0.02, 0.02));
	mesh->connectNameToMaterial("ChairFabric_CHR00402.jpg", chairFabric);



	/*Texture* tableTexture = new Texture();
	TextureImage* tableDiffuse = TextureImage::loadFromPFM("materials/table.pfm");
	tableTexture->setDiffuse(tableDiffuse);*/
	
	mesh->connectNameToMaterial("ChairWood_CHR00401.jpg", tableWood);



	/*
	// Chess Board
	*/
	Material* white = new Lambert(Vector3(0.97));
	Material* black = new Lambert(Vector3(0.03));
	mesh->connectNameToMaterial("ChessWhite", white);
	mesh->connectNameToMaterial("ChessBlack", black);

    Material* glassChessPieceWhite = new Glass(1.5, 0.9*Vector3(0.6, 0.6, 1), Vector3(0, 0, 0.1));
    Material* glassChessPieceBlack = new Glass(1.5, 0.9*Vector3(0.6, 1, 0.6), Vector3(0, 0.1, 0));
    mesh->connectNameToMaterial("ChessPiecesOne", glassChessPieceWhite);
    mesh->connectNameToMaterial("ChessPiecesTwo", glassChessPieceBlack);

	Wood* darkWood = new Wood(Vector3(45, 30, 18)/255.0, Vector3(49, 34, 22)/255.0, 0.3);
	darkWood->setGlossiness(300, 0.03, Vector3(1));
    mesh->connectNameToMaterial("ChessBoard", darkWood);

	/*
	// STAIRS
	*/

	// Light Sides

	Vector3 stairCaseBaseColor = Vector3(170, 85, 34)/255.0;
	Vector3 stairCaseHighColor = Vector3(218, 133, 34)/255.0;
	Material* staircaseSides = new Wood(stairCaseBaseColor, stairCaseHighColor, 0.53, Vector3(0,1,0), 1.36);
	mesh->connectNameToMaterial("StairsSides", staircaseSides);

	/*
	// SOFA AND TABLE
	*/
	//Material* sofaFabric = new Lambert(Vector3(1,0.1,0.1));
	mesh->connectNameToMaterial("SofaFabric", chairFabric);

	Material* sofaFeet = new Lambert(Vector3(0.8,0.8,0.1));
	mesh->connectNameToMaterial("SofaFeet", sofaFeet);

	//Material* frostedGlassTable = new FrostedGlass(Vector3(0.2), Vector3(0.9));
    mesh->connectNameToMaterial("TableTopGlass", new Glass(1.5, Vector3(0.9, 0.9, 1)));

	mesh->connectNameToMaterial("TableSides", new Lambert(Vector3(0.3, 0.3, 0.3)));

	mesh->connectNameToMaterial("TableMetalChrome", new Lambert(Vector3(0.3, 0.3, 0.3)));

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
