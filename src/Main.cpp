/*
* RayTracing pro GPUEngine
* Diplomova prace - master's thesis
* Bc. David Novák
* FIT VUT Brno
* 2018/2019
*
* Main.cpp - vstupni bod aplikace
*/

#pragma once
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#define STBI_MSC_SECURE_CRT
#include <iostream>
//#include "Scene.h"
//#include "Window.h"
//#include "RayTracing.h"

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 

#include <App.h>
#include <RayTracing.h>

#include <AssimpModelLoader.h>

#define APP_SUCCESS 0
#define APP_FAIL 1

int main(int, char**) {

	/*Scene sc;
	std::string f = "";
	std::string d = "";
	std::cout << "Path to file with scene:\n";
	std::cin >> f;
	sc.loadModel(f, d);
	
	Window w(1000, 800, "Stochastic Ray Tracing", true);*/

	//RayTracing rtx(sc, w);
	//rtx.run();
	/*AssimpModelLoader a;
	auto ms = a.loadScene("dragon.obj");
	if (ms != NULL) {

		for (auto m : ms->models) {
			for (auto n : m->meshes) {
				for (auto x : n->attributes) {
					//for (auto w : x->data);
				}
				
			}
		}


	}*/
		//std::cout << "sichs\n";

	
	try{
		App<RayTracing> a(1200, 800, "RayTracing");// (200, 700);
		a.run();
	}
	catch (std::exception e) {

		_CrtDumpMemoryLeaks();
		return APP_FAIL;
	}

	_CrtDumpMemoryLeaks();
	return APP_SUCCESS;
}