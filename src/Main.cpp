#include <iostream>
#include "Scene.h"
#include "Window.h"
#include "RayTracing.h"
#include "FPSCamera.h"
#include "CameraCallbacks.h"

//FPSCamera c;

int main() {

	Window w(1000, 800, "stRay", true);
	w.setCursorCallback(mouse_callback);
	//w.setKeyCallback(key_callback);

	Scene sc;
	std::string f = "../resource/plane/plane.obj";
	std::string d = "../resource/plane";
	sc.loadModel(f, d);
	
	RayTracing rt(sc, w);
	rt.run();

	//std::shared_ptr<ge::gl::Buffer> a = std::make_shared<ge::gl::Buffer>(1000*700*sizeof(float));
	//ge::gl::Buffer a(1000*700);
	//a->bindBase(GL_SHADER_STORAGE_BUFFER, 0);

	system("pause");

	return 0;
}