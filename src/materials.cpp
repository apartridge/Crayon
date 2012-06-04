/*
A file to test some materials
*/

#include "materials.h"
#include "materials/Lambert.h"
#include "materials/LightSource.h"
#include "lights/SquareLight.h"
#include "materials/Glass.h"
#include "materials/Wood.h"
#include "geometry/Sphere.h"
#include "materials/Metal.h"
#include "materials/Fabric.h"

void makeWoodScene() 
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;
    g_image->resize(720, 480);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(-2, 10, -2));
    g_camera->setLookAt(Vector3(-5, 0, -5));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

	g_camera->setFocalLength(0*15);
	g_camera->setAperture(0*0.7);

    // Create light sources

    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 8, 10));

    light->setColor(Vector3(1, 1, 1));
    light->setPower(80);
    //g_scene->addLight(light);

	PointLight * light2 = new PointLight;
    light2->setPosition(Vector3(10, 7, -10));

    light2->setColor(Vector3(1, 1, 1));
    light2->setPower(80);
    //g_scene->addLight(light2);

	PointLight * light4 = new PointLight;
    light4->setPosition(Vector3(-3, 9, -3));
    light4->setColor(Vector3(1, 1, 1));
    light4->setPower(80);
    g_scene->addLight(light4);

	Material* material0 = new Lambert(0.2*Vector3(1.0f, 1.0f, 1.0f));
    Material* material1 = new Lambert(0.9*Vector3(1.0f, 1.0f, 1.0f));
    Material* material2 = new Lambert(Vector3(0, 1.0f, 0));
    
	
	/*
	// Wood Medium with some gloss
	*/
	
	Vector3 baseColor = Vector3(33, 16, 8)/255.0;
	Vector3 highColor = Vector3(132, 82, 49)/255.0;
	const float scale = 0.54;
	Wood* materialWood = new Wood(baseColor, highColor, scale);

	Vector3 glossColor = Vector3(1);
	int glossPower = 250;
	float glossFactor = 0.01;
	materialWood->setGlossiness(glossPower, glossFactor, glossColor);





	/*
	// Wood Light
	*/
	
	Vector3 baseColor2 = Vector3(170, 85, 34)/255.0;
	Vector3 highColor2 = Vector3(218, 133, 34)/255.0;

	const float scale2 = 0.53;
	Material* materialWood2 = new Wood(baseColor2, highColor2, scale2);




	/*
	// Wood Dark
	*/
	
	Vector3 baseColor3 = Vector3(45, 30, 18)/255.0;
	Vector3 highColor3 = Vector3(123, 82, 49)/255.0;

	const float scale3 = 0.3;
	Material* materialWood3 = new Wood(baseColor3, highColor3, scale3);



    // Add spheres
    /*const float radius = 1;
    Material *glass[] = {new Glass(), new Glass()};
    for (int i = 0; i < 2; ++i)
    {
        Sphere* sphere = new Sphere();
        sphere->setRadius(radius);
        sphere->setCenter(Vector3(-5/2.0 + 5*i, radius, -5/3.0 + 5/2.0*i));
        sphere->setMaterial(glass[i]);
        g_scene->addObject(sphere);
    }*/
	
	TriangleMesh * mesh = new TriangleMesh;
	mesh->setDefaultMaterial(material0);

	mesh->connectNameToMaterial("Glass", new Fabric(Vector3(1,0,0)));


	mesh->connectNameToMaterial("TableWood_FW_WD_BB.JPG", materialWood);



	mesh->connectNameToMaterial("MaterialTwo", materialWood2);
	mesh->connectNameToMaterial("MaterialThree", materialWood3);
    mesh->load("table.obj");
	g_scene->addMesh(mesh);


    g_scene->preCalc();
}

void makeLeatherScene() 
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

	g_camera->setFocalLength(0);
	g_camera->setAperture(0.0);

    // Create light sources

    PointLight * light = new PointLight;
    light->setPosition(Vector3(10, 8, 10));

    light->setColor(Vector3(1, 1, 1));
    light->setPower(55);
    g_scene->addLight(light);

	PointLight * light2 = new PointLight;
    light2->setPosition(Vector3(10, 8, -10));

    light2->setColor(Vector3(1, 1, 1));
    light2->setPower(55);
    g_scene->addLight(light2);


	PointLight * light3 = new PointLight;
    light3->setPosition(Vector3(3, -4, 9));

    light3->setColor(Vector3(1, 1,1 ));
    light3->setPower(55);
    g_scene->addLight(light3);

	PointLight * light4 = new PointLight;
    light4->setPosition(Vector3(0, 3, 0));

    light4->setColor(Vector3(1, 1, 1));
    light4->setPower(30);
    g_scene->addLight(light4);

	Material* material1 =  new Lambert(Vector3(1.0, 0.0f, 0));


    Material* material0 = new Lambert(Vector3(0, 0.0f, 1));
	Material* materialMetal = new Metal(Vector3(198.0/255.0), Vector3(225.0/255.0), 0.1 );


    Material* material2 = new Lambert(Vector3(0, 1.0f, 0));
    
	

	TriangleMesh * mesh = new TriangleMesh;
	mesh->setDefaultMaterial(material0);
	//mesh->connectNameToMaterial("Glass", new FrostedGlass(Vector3(0.2), Vector3(0.9, 0.9, 1.0)));


	mesh->connectNameToMaterial("MaterialOne", material0);
	mesh->connectNameToMaterial("MaterialTwo", materialMetal);
	mesh->connectNameToMaterial("MaterialThree", material1);
    mesh->load("monkey.obj");
	g_scene->addMesh(mesh);


    g_scene->preCalc();
}
