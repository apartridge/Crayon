/*
A file to test some materials
*/

#include "materials.h"
#include "materials/Lambert.h"
#include "materials/LightSource.h"
#include "lights/SquareLight.h"
#include "materials/Glass.h"
#include "materials/Stone.h"

void makeWoodScene() 
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;
    g_image->resize(1280, 720);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(3, 5, 10));
    g_camera->setLookAt(Vector3(0, 3, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // Create light sources

    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 8, 10));

    light->setColor(Vector3(1, 1, 1));
    light->setPower(25);
    g_scene->addLight(light);

	PointLight * light2 = new PointLight;
    light2->setPosition(Vector3(10, 8, -10));

    light2->setColor(Vector3(1, 1, 1));
    light2->setPower(25);
    g_scene->addLight(light2);


	PointLight * light3 = new PointLight;
    light3->setPosition(Vector3(3, -4, 9));

    light3->setColor(Vector3(1, 1,1 ));
    light3->setPower(25);
    g_scene->addLight(light3);

	PointLight * light4 = new PointLight;
    light4->setPosition(Vector3(0, 3, 0));

    light4->setColor(Vector3(1, 1, 1));
    light4->setPower(10);
    g_scene->addLight(light4);

	Material* material0 = new Lambert(0.2*Vector3(1.0f, 1.0f, 1.0f));
    Material* material1 = new Lambert(0.9*Vector3(1.0f, 1.0f, 1.0f));
    Material* material2 = new Lambert(Vector3(0, 1.0f, 0));
    Material* material3 = new Stone();
	
	TriangleMesh * mesh = new TriangleMesh;
	mesh->setDefaultMaterial(material0);
	mesh->connectNameToMaterial("MaterialOne", material1);
	mesh->connectNameToMaterial("MaterialTwo", material2);
	mesh->connectNameToMaterial("MaterialThree", material3);
    mesh->load("monkey.obj");
	g_scene->addMesh(mesh);


    g_scene->preCalc();
}


