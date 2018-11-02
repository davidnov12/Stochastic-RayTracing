#pragma once
#include <iostream>
#include "Scene.h"
#include "Window.h"
#include "RayTracing.h"
//#include "FPSCamera.h"
//#include "CameraCallbacks.h"

//FPSCamera c;

int main() {

	Window w(1000, 800, "stRay", true);
	//w.setCursorCallback(mouse_callback);
	//w.setKeyCallback(key_callback);

	Scene sc;
	std::string f = "../resource/plane/plane.obj";
	std::string d = "../resource/plane";
	sc.loadModel(f, d);
	
	RayTracing rt(sc, w);
	rt.run();

	return 0;
}