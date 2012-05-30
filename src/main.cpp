#include <math.h>
#include "assignment1.h"
#include "assignment2.h"
#include "finalsproject.h"
#include "materials.h"
#include "renderer/Window.h"
#include <iostream>
#include "sysutils/Random.h"


int main(int argc, char*argv[])
{
	Random::seed();

	printf("Crayon Raytracer Main Menu:\n");
	printf("1) Finals Project\n");
	printf("2) 20 Bunnies\n");
	printf("3) 2 Bunnies\n");
	printf("4) Sponza\n");
	printf("5) Teapot\n");
	printf("6) Dragon Scene\n");
	printf("7) Cornell Scene\n");
	printf("8) Materials: Wood\n");
    printf("9) Cornell Scene (glass balls)\n");
	printf("0) Materials: Glass\n");


	int option = 0;

	if(option == 0)
		std::cin >> option;

	switch(option)
	{
	case 1:
		makeFinalScene();
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
		makeWoodScene();
		break;
	case 9:
		makeCornellSceneGlass();
		break;
	}


	Window miro(&argc, argv);
	miro.mainLoop();



    return 0; 

}

