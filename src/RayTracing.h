/*

*/
#pragma once

#include <Scene.h>
#include <Window.h>
#include <Renderer.h>
#include <FPScameraManager.h>

#include <iostream>
#include <fstream>
#include <thread>

#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>

#ifndef COMPUTE_SHADER_PATH
#define COMPUTE_SHADER_PATH "../shaders/trace.cs"
#endif

#ifndef VERTEX_SHADER_PATH
#define VERTEX_SHADER_PATH "../shaders/display.vs"
#endif

#ifndef FRAGMENT_SHADER_PATH
#define FRAGMENT_SHADER_PATH "../shaders/display.fs"
#endif


class RayTracing : public Renderer {

public:

	RayTracing(){}

	~RayTracing(){
		ge::gl::glDeleteTextures(1, &renderTexture);
	}

	void init() override;
	void setWindowObject(std::shared_ptr<Window> w) override;
	void render() override;
	void updateScene(Scene& s) override;
	
private:

	bool drawFrame = true;
	std::shared_ptr<Window> win;
	std::shared_ptr<ge::gl::Program> display;
	std::shared_ptr<ge::gl::Program> tracer;
	std::shared_ptr<FPSCameraManager> camera;

	std::shared_ptr<ge::gl::Buffer> geomBuff;
	std::shared_ptr<ge::gl::Buffer> matBuff;
	std::shared_ptr<ge::gl::Buffer> renderBuff;
	std::shared_ptr<ge::gl::Buffer> nodeBuff;
	std::shared_ptr<ge::gl::Buffer> indBuff;
	GLuint renderTexture;

};

/*class RayTracing {

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

	std::shared_ptr<ge::gl::Buffer> geomBuff, matBuff, renderBuff, nodeBuff, indBuff;
	std::shared_ptr<ge::gl::Texture> renderTex;
	GLuint query;
	GLuint64 elapsed_time;
	GLuint renTex;
	int done = 0;

};*/