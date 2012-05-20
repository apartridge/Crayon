#include <math.h>
#include "../headers/assignment1.h"
#include "../headers/assignment2.h"
#include "../headers/renderer/Window.h"
#include <iostream>
#include "../headers/sysutils/Random.h"

void spheres()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(1280, 720);


    // set up the camera

    g_camera->setBGColor(Vector3(0.f, 0.f, 0.f));
    g_camera->setEye(Vector3(-4, 2, 2));
    g_camera->setLookAt(Vector3(0, 1, -2));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source

    PointLight* light = new PointLight;
    light->setPosition(Vector3(-5, 10, 10));
    light->setColor(Vector3(1, 1, 1));
    light->setPower(20);
    g_scene->addLight(light);

	 PointLight* light2 = new PointLight;
    light2->setPosition(Vector3(5, 10, -10));
    light2->setColor(Vector3(1, 1, 1));
    light2->setPower(300);
    g_scene->addLight(light2);


	/*
	Spheres
	*/

	Sphere* sphere1 = new Sphere;
    sphere1->setCenter(Vector3(-1.25, 1.25, -2.5));
    sphere1->setRadius(1.25);
	Shiny* material1 = new Shiny(Vector3(0, 0, 0), Medium(1.5f), 0.0, Vector3(0.0, 0.0, 0.0), 0.0);
	sphere1->setMaterial(material1);
    g_scene->addObject(sphere1);

	Sphere* sphere2 = new Sphere;
    sphere2->setCenter(Vector3(-0.5, 0.75, 0));
    sphere2->setRadius(0.75);
	Shiny* material2 = new Shiny(Vector3(1, 1, 0), Medium(1.5f), 0, Vector3(0.01, 0.01, 0.0), 0.0);
	sphere2->setMaterial(material2);
    g_scene->addObject(sphere2);


	Sphere* sphere3 = new Sphere;
    sphere3->setCenter(Vector3(1.25, 1.25, -1.25));
    sphere3->setRadius(1.25);
	Shiny* material3 = new Shiny(Vector3(0, 1, 0), Medium(1.5f), 0, Vector3(0.0, 0.01, 0.0), 0.0);
	sphere3->setMaterial(material3);
    g_scene->addObject(sphere3);

	Sphere* sphere4 = new Sphere;
    sphere4->setCenter(Vector3(-2, 0.5, -0.5));
    sphere4->setRadius(0.5);
	Shiny* material4 = new Shiny(Vector3(0, 0, 1), Medium(1.5f), 0.1, Vector3(0.0, 0.01, 0.0), 0.0);
	sphere4->setMaterial(material4);
    g_scene->addObject(sphere4);


	/*
	Create the floor triangle
	*/

    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();

    floor->setV1(Vector3(  0, 0,  10));
    floor->setV2(Vector3( 10, 0, -10));
    floor->setV3(Vector3(-10, 0, -10));

    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));

    Shiny* groundmirrormat = new Shiny(Vector3(88.0/255, 79.0/255, 71.0/255), Medium(1.4f), 1, 0.10*Vector3(0.5,0.5,0.5), 0.1);

    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(groundmirrormat); 
    g_scene->addObject(t);

	// Skydome

	/*Sphere* skydome = new Sphere;
    skydome->setCenter(Vector3(0,0,0));
    skydome->setRadius(100);
	Skydome* skydome_material = new Skydome("stpeters_probe.pfm"); // GREEN
	skydome->setMaterial(skydome_material);
    //g_scene->addObject(skydome);
	
    // let objects do pre-calculations if needed
    g_scene->preCalc();*/

}


int main(int argc, char*argv[])
{
	Random::seed();

	printf("Raytracer Main Menu:\n");
	printf("1) One Bunny\n");
	printf("2) 20 Bunnies\n");
	printf("3) 2 Bunnies\n");
	printf("4) Sponza\n");
	printf("5) Teapot\n");
	printf("6) Dragon Scene\n");
	printf("7) Cornell Scene\n");
	printf("8) Spheres\n");
    printf("9) Cornell Scene (glass balls)\n");


	int option = 0;

	if(option == 0)
		std::cin >> option;

	switch(option)
	{
	case 1:
		makeBunny1Scene();
		break;
	case 2:
		makeBunny20Scene();
		break;
	case 3:
		makeDualScene();
		break;
	case 4:
		makeSponzaScene();
		break;
	case 5:
		makeTeapotScene();
		break;
	case 6:
		makeDragonScene();
		break;
	case 7:
		makeCornellScene();
		break;
	case 8:
		spheres();
		break;
	case 9:
		makeCornellSceneGlass();
		break;
	}


	Window miro(&argc, argv);
	miro.mainLoop();



    return 0; 

}

