// #include "assignment1.h"

/*
void makeBunnyScene()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    g_image->resize(128, 128);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(-2, 3, 5));
    g_camera->setLookAt(Vector3(-.5, 1, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-3, 15, 3));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(500);
    g_scene->addLight(light);

    Shiny* mat = new Shiny(Vector3(1.0f), Medium(1.5), 0.3 );

    TriangleMesh * bunny = new TriangleMesh;
    bunny->load("bunny.obj");
    
    // create all the triangles in the bunny mesh and add to the scene
    for (int i = 0; i < bunny->numTris(); ++i)
    {
        Triangle* t = new Triangle;
        t->setIndex(i);
        t->setMesh(bunny);
        t->setMaterial(mat); 
        g_scene->addObject(t);
    }
    
    // create the floor triangle
    TriangleMesh * floor = new TriangleMesh;
    floor->createSingleTriangle();
    floor->setV1(Vector3(  0, 0,  10));
    floor->setV2(Vector3( 10, 0, -10));
    floor->setV3(Vector3(-10, 0, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(mat); 
    g_scene->addObject(t);
    
    // let objects do pre-calculations if needed
    g_scene->preCalc();
}




void makeTeapotSceneS()
{
    g_camera = new Camera;
    g_scene = new Scene;
    g_image = new Image;

    //g_image->resize(300, 300);
    
    // set up the camera
    g_camera->setBGColor(Vector3(0.0f, 0.0f, 0.2f));
    g_camera->setEye(Vector3(-2, 3, 5));
    g_camera->setLookAt(Vector3(-.5, 1, 0));
    g_camera->setUp(Vector3(0, 1, 0));
    g_camera->setFOV(45);

    // create and place a point light source
    PointLight * light = new PointLight;
    light->setPosition(Vector3(-3, 15, 3));
    light->setColor(Vector3(1, 1, 1));
    light->setWattage(500);
    g_scene->addLight(light);

    Material* mat = new Shiny(Vector3(1.0f), Medium(1.5), 0.0, 0.0);

    TriangleMesh * sphere = new TriangleMesh;
    sphere->load("sphere_smooth.obj");
    
    // create all the triangles in the mesh and add to the scene
    for (int i = 0; i < sphere->numTris(); ++i)
    {
        Triangle* t = new Triangle;
        t->setIndex(i);
        t->setMesh(sphere);
        t->setMaterial(mat); 
        g_scene->addObject(t);
    }
    
    // create the floor triangle

	Material* mat2 = new Shiny(Vector3(1.0f, 0, 0), Medium(1), 1, Vector3(0.1, 0, 0), 0.3);

    TriangleMesh * floor = new TriangleMesh;

    floor->createSingleTriangle();
    floor->setV1(Vector3(  0, 0,  10));
    floor->setV2(Vector3( 10, 0, -10));
    floor->setV3(Vector3(-10, 0, -10));
    floor->setN1(Vector3(0, 1, 0));
    floor->setN2(Vector3(0, 1, 0));
    floor->setN3(Vector3(0, 1, 0));
    
    Triangle* t = new Triangle;
    t->setIndex(0);
    t->setMesh(floor);
    t->setMaterial(mat2); 
    g_scene->addObject(t);
    

	
	Sphere* skydome = new Sphere;
    skydome->setCenter(Vector3(0,0,0));
    skydome->setRadius(100);
	Skydome* skydome_material = new Skydome("stpeters_probe.pfm"); // GREEN
	skydome->setMaterial(skydome_material);
    //g_scene->addObject(skydome);


    // let objects do pre-calculations if needed
    g_scene->preCalc();
}
*/