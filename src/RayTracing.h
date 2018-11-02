#pragma once
#include "Scene.h"
#include "Window.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>


class RayTracing {

public:

	RayTracing(Scene& sc, Window& win) :s(sc), w(win) { init(); }

	void run();

private:
	
	void drawFrame();
	void rayTrace();

	void init();

	Scene& s;
	Window& w;

	unsigned int activeBuffer;
	bool stopDraw = false;
	std::shared_ptr<ge::gl::Program> display;
	std::shared_ptr<ge::gl::Program> tracer;

	std::shared_ptr<ge::gl::Buffer> geomBuff, matBuff, renderBuff;
	std::shared_ptr<ge::gl::Texture> renderTex;
	GLuint renTex;

};