#include "../headers/assignment2.h"
#include "../headers/materials/Lambert.h"
#include "../headers/materials/LightSource.h"
#include "../headers/lights/SquareLight.h"

#include "../headers/materials/Glass.h"

// local helper function declarations
namespace
{
	void addMeshTrianglesToScene(TriangleMesh * mesh, Material * material);
	inline Matrix4x4 translate(float x, float y, float z);
	inline Matrix4x4 scale(float x, float y, float z);
	inline Matrix4x4 rotate(float angle, float x, float y, float z);
}

void makeTeapotScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(0, 3, 6));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 10, 10));
    light->setColor(Vector3(1, 1, 1));
    light->setPower(700);
    g_scene->addLight(light);

    Material* material = new Lambert(Vector3(1.0f));
    TriangleMesh * teapot = new TriangleMesh;
    teapot->load("teapot.obj");
    addMeshTrianglesToScene(teapot, material);
    
    // Extra teapot
    Matrix4x4 xform;
    xform.setIdentity();
    xform *= translate(7.6, .8, .6);
    TriangleMesh *teapot2 = new TriangleMesh;
    teapot2->load("teapot.obj", xform);
    addMeshTrianglesToScene(teapot2, material);
    
    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(-10, 0, -10));
    floor->setV2(Vector3(  0, 0,  10));
    floor->setV3(Vector3( 10, 0, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(material); 
    g_scene->addObject(t);
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();

    /*
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(0, 3, 6));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 10, 10));
    light->setColor(Vector3(1, 1, 1));
    light->setPower(700);
    g_scene->addLight(light);

    Material* material = new Lambert(Vector3(1.0f));

	TriangleMesh* teapot_mesh = new TriangleMesh;
	teapot_mesh->load("teapot.obj");

    addMeshTrianglesToScene(teapot_mesh, material);
    
    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();


    floor->setV1(Vector3(-20, 0, -20));
    floor->setV2(Vector3(  0, 0,  0));
    floor->setV3(Vector3( 0, 0, -20));


    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);


	Material* material2 = new Lambert(Vector3(0, 1.0f, 0 ));
    t->setMaterial(material2); 
	g_scene->addObject(t);
	g_scene->addMesh(floor);

    // let objects do pre-calculations if needed
    g_scene->preCalc();*/
}

void makeDualScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(0, 3, 6));
    g_camera->setLookAt(Vector3(0, 2, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 10, 10));
    light->setColor(Vector3(1, 1, 1));
    light->setPower(700);
    g_scene->addLight(light);

     Matrix4x4 xform;
    Matrix4x4 xform2;
    xform2 *= rotate(110, 0, 1, 0);
    xform2 *= scale(.6, 1, 1.1);
	
	Material* material = new Lambert(Vector3(1.0f));
	TriangleMesh * mesh;

    // bunny 1
    xform.setIdentity();
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 2
    xform.setIdentity();
	xform *= translate(2, 0, 0);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(-10, 0, -10));
    floor->setV2(Vector3(  0, 0,  10));
    floor->setV3(Vector3( 10, 0, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);

	Material* material2 = new Lambert(Vector3(0, 1.0f, 0 ));
    t->setMaterial(material2); 
    //g_scene->addObject(t);
	//g_scene->addMesh(floor);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void makeBunny1Scene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(0, 5, 15));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 20, 10));
    light->setColor(Vector3(1, 1, 1));
    light->setPower(1000);
    g_scene->addLight(light);

    Material *notShiny = new Lambert(1);
    Material* material = notShiny; //new Shiny(Vector3(1.0f), Medium(1.5), 0.0 );

    TriangleMesh * bunny = new TriangleMesh;
    bunny->load("bunny.obj");
    addMeshTrianglesToScene(bunny, material);
    
    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(-100, 0, -100));
    floor->setV2(Vector3(   0, 0,  100));
    floor->setV3(Vector3( 100, 0, -100));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
	Material* material2 = new Lambert(Vector3(1,1,1));
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(material2); 
    g_scene->addObject(t);
	g_scene->addMesh(floor);
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void makeDragonScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(0, 5, 15));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 20, 10));
    light->setColor(Vector3(1, 1, 1));
    light->setPower(1000);
    g_scene->addLight(light);

    Material * notShiny = new Lambert(1);
    Material* material = notShiny; //new Shiny(Vector3(1.0f), Medium(1.4), 0.0, 0.0);
    TriangleMesh * dragon = new TriangleMesh;
    dragon->load("dragon2.obj");
    addMeshTrianglesToScene(dragon, material);

	Matrix4x4 xform;
	xform.setIdentity();
    xform *= translate(-7, 0, -3);
	Material* material3 = new Lambert(Vector3(1,0,0));
    TriangleMesh * dragon2 = new TriangleMesh;
    dragon2->load("dragon2.obj", xform);
    addMeshTrianglesToScene(dragon2, material3);

	xform.setIdentity();
    xform *= translate(6, 0, -3);
	Material* material4 = notShiny; //new Shiny(Vector3(0,1,0), Medium(1), 1, 0.1);
    TriangleMesh * dragon3 = new TriangleMesh;
    dragon3->load("dragon2.obj", xform);
    addMeshTrianglesToScene(dragon3, material4);

	xform.setIdentity();
    xform *= translate(0, 4, 4);
	xform *= scale(1,1,1);
	Material* matsphere = notShiny; //new Shiny(Vector3(1.0f), Medium(1.5), 0.0, 0.0);
    TriangleMesh * sphere = new TriangleMesh;
    sphere->load("sphere3.obj", xform);
    addMeshTrianglesToScene(sphere, matsphere);

    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(-100, 0, -100));
    floor->setV2(Vector3(   0, 0,  100));
    floor->setV3(Vector3( 100, 0, -100));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
	Material* material2 = new Lambert(Vector3(1,1,1));
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(material2); 
    g_scene->addObject(t);
	g_scene->addMesh(floor);

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void makeBunny20Scene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(0, 5, 15));
    g_camera->setLookAt(Vector3(0, 0, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 20, 10));
    light->setColor(Vector3(1, 1, 1));
    light->setPower(1000);
    g_scene->addLight(light);

    TriangleMesh * mesh;
    Material* material = new Lambert(Vector3(1.0f));
    Matrix4x4 xform;
    Matrix4x4 xform2;
    xform2 *= rotate(110, 0, 1, 0);
    xform2 *= scale(.6, 1, 1.1);


    // bunny 1
    xform.setIdentity();
    xform *= scale(0.3, 2.0, 0.7);
    xform *= translate(-1, .4, .3);
    xform *= rotate(25, .3, .1, .6);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 2
    xform.setIdentity();
    xform *= scale(.6, 1.2, .9);
    xform *= translate(7.6, .8, .6);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 3
    xform.setIdentity();
    xform *= translate(.7, 0, -2);
    xform *= rotate(120, 0, .6, 1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 4
    xform.setIdentity();
    xform *= translate(3.6, 3, -1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 5
    xform.setIdentity();
    xform *= translate(-2.4, 2, 3);
    xform *= scale(1, .8, 2);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 6
    xform.setIdentity();
    xform *= translate(5.5, -.5, 1);
    xform *= scale(1, 2, 1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 7
    xform.setIdentity();
    xform *= rotate(15, 0, 0, 1);
    xform *= translate(-4, -.5, -6);
    xform *= scale(1, 2, 1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 8
    xform.setIdentity();
    xform *= rotate(60, 0, 1, 0);
    xform *= translate(5, .1, 3);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 9
    xform.setIdentity();
    xform *= translate(-3, .4, 6);
    xform *= rotate(-30, 0, 1, 0);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 10
    xform.setIdentity();
    xform *= translate(3, 0.5, -2);
    xform *= rotate(180, 0, 1, 0);
    xform *= scale(1.5, 1.5, 1.5);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 11
    xform = xform2;
    xform *= scale(0.3, 2.0, 0.7);
    xform *= translate(-1, .4, .3);
    xform *= rotate(25, .3, .1, .6);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 12
    xform = xform2;
    xform *= scale(.6, 1.2, .9);
    xform *= translate(7.6, .8, .6);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 13
    xform = xform2;
    xform *= translate(.7, 0, -2);
    xform *= rotate(120, 0, .6, 1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 14
    xform = xform2;
    xform *= translate(3.6, 3, -1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 15
    xform = xform2;
    xform *= translate(-2.4, 2, 3);
    xform *= scale(1, .8, 2);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 16
    xform = xform2;
    xform *= translate(5.5, -.5, 1);
    xform *= scale(1, 2, 1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 17
    xform = xform2;
    xform *= rotate(15, 0, 0, 1);
    xform *= translate(-4, -.5, -6);
    xform *= scale(1, 2, 1);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 18
    xform = xform2;
    xform *= rotate(60, 0, 1, 0);
    xform *= translate(5, .1, 3);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 19
    xform = xform2;
    xform *= translate(-3, .4, 6);
    xform *= rotate(-30, 0, 1, 0);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);

    // bunny 20
    xform = xform2;
    xform *= translate(3, 0.5, -2);
    xform *= rotate(180, 0, 1, 0);
    xform *= scale(1.5, 1.5, 1.5);
    mesh = new TriangleMesh;
    mesh->load("bunny.obj", xform);
    addMeshTrianglesToScene(mesh, material);


    // create the floor triangle
    mesh = new TriangleMesh;
    mesh->createSingleTriangle();
    mesh->setV1(Vector3(-100, 0, -100));
    mesh->setV2(Vector3(   0, 0,  100));
    mesh->setV3(Vector3( 100, 0, -100));
    mesh->setN1(Vector3(0, 1, 0));
    mesh->setN2(Vector3(0, 1, 0));
    mesh->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(mesh);
    t->setMaterial(material); 
    g_scene->addObject(t);
	g_scene->addMesh(mesh);
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void makeSponzaScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(8, 1.5, 1));
    g_camera->setLookAt(Vector3(0, 2.5, -1));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(55);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(0, 10.0, 0));
    light->setColor(Vector3(1, 1, 1));
    light->setPower(200);
    g_scene->addLight(light);

    Material* material = new Lambert(Vector3(1.0f));
    TriangleMesh * mesh = new TriangleMesh;
    mesh->load("sponza.obj");
    addMeshTrianglesToScene(mesh, material);
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void makeCornellScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(512, 512);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(3, 3, 7));
    g_camera->setLookAt(Vector3(3, 3, -2));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(55);

    // create and place a point light source
    /*PointLight * light = new PointLight;
    light->setPosition(Vector3(3, 5, -3));
    light->setColor(Vector3(1, 1, 1));
    light->setPower(50);
    g_scene->addLight(light);*/

    Material* materialWhite = new Lambert(Vector3(0.4f, 0.4f, 0.4f));
	Material* materialRed = new Lambert(Vector3(1.0f, 0, 0));
	Material* materialGreen = new Lambert(Vector3(0, 1.0f, 0));
	Material* materialCeiling = new Lambert(Vector3(0.4f, 0.4f, 0.4f));


    TriangleMesh * mesh = new TriangleMesh ();
	//mesh->connectMaterialToName( materialRed, "Cornell_Box_-_Red");
    mesh->load("cornell_box.obj");
	//mesh->addMeshToScene(g_scene);
	addMeshTrianglesToScene(mesh, materialWhite);

	g_scene->objects()->at(4)->setMaterial(materialRed);
	g_scene->objects()->at(5)->setMaterial(materialRed);

	g_scene->objects()->at(6)->setMaterial(materialGreen);
	g_scene->objects()->at(7)->setMaterial(materialGreen);

	g_scene->objects()->at(8)->setMaterial(materialCeiling);
	g_scene->objects()->at(9)->setMaterial(materialCeiling);
	g_scene->objects()->at(10)->setMaterial(materialCeiling);
	g_scene->objects()->at(11)->setMaterial(materialCeiling);
	g_scene->objects()->at(12)->setMaterial(materialCeiling);
	g_scene->objects()->at(13)->setMaterial(materialCeiling);
	g_scene->objects()->at(14)->setMaterial(materialCeiling);
	g_scene->objects()->at(15)->setMaterial(materialCeiling);

	// Roof Light
	Matrix4x4 xform;
	xform.setIdentity();
    xform *= translate(2.7, 5.6, -2.7);
	xform *= scale(0.8,0.8,0.8);

	Material* arealight = new LightSource();
    TriangleMesh * lll = new TriangleMesh;
    lll->load("square.obj", xform);
    addMeshTrianglesToScene(lll, arealight);

	SquareLight * light = new SquareLight;
    light->setColor(Vector3(1, 1, 1));
    light->setPower(10);
	light->setPosition( Vector3(2.5, 5.55, -3), Vector3(3.0, 5.55, -2.5) ) ;
	g_scene->addLight(light);


	// Ball
    /*
	Sphere* sphere3 = new Sphere;
    sphere3->setCenter(Vector3(3.7, 2.10, -1.5));
    sphere3->setRadius(0.5);
	Shiny* material3 = new Shiny(Vector3(1, 1, 1), Medium(1.5f), 1, Vector3(0.0, 0.01, 0.0), 1.0);
	sphere3->setMaterial(material3);
    g_scene->addObject(sphere3);
    */

	
	Matrix4x4 xformdra;
	xformdra.setIdentity();
	xformdra *= translate(1.3,0,-1.5);
    xformdra *= scale(0.15,0.15,0.15);
	Material* matdragon = new Glass();
    TriangleMesh * dragon2 = new TriangleMesh;
    dragon2->load("dragon2.obj", xformdra);
   // addMeshTrianglesToScene(dragon2, matdragon);
    
	// Ball 2

	/*Sphere* sphere2 = new Sphere;
    sphere2->setCenter(Vector3(1.5, 0.8, -0.6));
    sphere2->setRadius(0.8);
	Shiny* material2 = new Shiny(Vector3(0, 0, 0), Medium(1.5f), 0, Vector3(0.0, 0.01, 0.0), 0);
	sphere2->setMaterial(material2);
    g_scene->addObject(sphere2);*/

    // let objects do pre-calculations if needed
    g_scene->preCalc();
}

void makeCornellSceneGlass() 
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;
    g_image->resize(512, 512);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(0, 3, 9));
    g_camera->setLookAt(Vector3(0, 2, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // Specify cube size
    const float w = 3, d = 4, h = 5;

    // Create light source
    if (false)
    {
        /*const float scale = 1/3.0;
        Vector3 vertices[] = { 
            Vector3(-w*scale, h - epsilon, d*scale),
            Vector3(-w*scale, h - epsilon, -d*scale),
            Vector3(w*scale, h - epsilon, -d*scale),
            Vector3(w*scale, h - epsilon, d*scale)
        };
        LightSource * areaLight = new AreaLight(vertices, *g_scene);
        g_scene->addLight(areaLight);*/
    }
    else
    {
        // create and place a point light source
        PointLight * light = new PointLight;
        light->setPosition(Vector3(0, h - 0.5, 0));
        light->setColor(Vector3(1, 1, 1));
        light->setPower(10);
        g_scene->addLight(light);
    }

    // Create cube
    Vector3 *vertex = new Vector3[8];
    vertex[0] = Vector3(-w, 0, d); // Bottom 4
    vertex[1] = Vector3(-w, 0, -d);
    vertex[2] = Vector3(w, 0, -d);
    vertex[3] = Vector3(w, 0, d);
    vertex[4] = Vector3(-w, h, d); // Top 4
    vertex[5] = Vector3(-w, h, -d);
    vertex[6] = Vector3(w, h, -d);
    vertex[7] = Vector3(w, h, d);

    Vector3 *normal = new Vector3[5];
    normal[0] = Vector3(0, 1, 0); // Floor
    normal[1] = Vector3(1, 0, 0); // Left wall
    normal[2] = Vector3(0, 0, 1); // Back wall
    normal[3] = Vector3(-1, 0, 0); // Right wall
    normal[4] = Vector3(0, -1, 0); // Ceiling

    TriangleMesh *cubeMesh = new TriangleMesh(vertex, normal, 10);

    int vertexIndices[10][3] = {{0,1,2}, {0,2,3}, {0,4,1}, {4,5,1}, {1,5,2}, {5,6,2}, {2,6,7}, {2,7,3}, {4,5,6}, {4,6,7}};
    int normalIndices[10] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4};

    Material *red = new Lambert(Vector3(1, 0.5, 0.5));
    Material *blue = new Lambert(Vector3(0.5, 0.5, 1));
    Material *gray = new Lambert(1.0);

    for (int i = 0; i < 10; i++)
    {
        Triangle *t = cubeMesh->addTriangle(vertexIndices[i][0], vertexIndices[i][1], vertexIndices[i][2], normalIndices[i]);
        if (i == 2 || i == 3)
            t->setMaterial(red);
        else if (i == 6 || i == 7)
            t->setMaterial(blue);
        else
            t->setMaterial(gray);
        g_scene->addObject(t);
    }

    // Add spheres
    const float radius = 1;
    Material *glass = new Glass();
    for (int i = 0; i < 2; ++i)
    {
        Sphere* sphere = new Sphere();
        sphere->setRadius(radius);
        sphere->setCenter(Vector3(-w/2.0 + w*i, radius, -d/3.0 + d/2.0*i));
        sphere->setMaterial(glass);
        g_scene->addObject(sphere);
    }

    g_scene->preCalc();
}



// local helper function definitions
namespace
{
	void addMeshTrianglesToScene(TriangleMesh * mesh, Material * material)
	{

		for (int i = 0; i < mesh->numTris(); ++i)
		{
			Triangle* t = new Triangle;
			t->setIndex(i);
			t->setMesh(mesh);
			t->setMaterial(material);
			g_scene->addObject(t);
		}
	}

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
	}

}
