#include "RayTracing.h"

void RayTracing::run(){

	while (!w.isClosed()) {

		ge::gl::glClearColor(0, 1, 0.7, 1);
		ge::gl::glClear(GL_COLOR_BUFFER_BIT);

		// Ray trace
		tracer->use();
		geomBuff->bindBase(GL_SHADER_STORAGE_BUFFER, 0);
		matBuff->bindBase(GL_SHADER_STORAGE_BUFFER, 2);
		renderBuff->bindBase(GL_SHADER_STORAGE_BUFFER, 1);
		//renderTex->bindImage(0);
		
		//ge::gl::glDispatchCompute(125, 100, 1);
		ge::gl::glDispatchCompute(1000, 800, 1);
		ge::gl::glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
		
		//renderTex->unbind(0);
		geomBuff->unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
		matBuff->unbindBase(GL_SHADER_STORAGE_BUFFER, 2);
		renderBuff->unbindBase(GL_SHADER_STORAGE_BUFFER, 1);
		// Ray trace

		// Draw frame
		display->use();
		renderBuff->bindBase(GL_SHADER_STORAGE_BUFFER, 0);
		ge::gl::glActiveTexture(GL_TEXTURE0);
		ge::gl::glBindTexture(GL_TEXTURE_2D, renTex);
		//renderTex->bind(0);
		ge::gl::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//renderTex->unbind(0);
		renderBuff->unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
		// Draw frame

		w.swapBuffers();
	}

}

void RayTracing::drawFrame(){

	while (!w.isClosed()) {
		glfwPollEvents();
		std::cout << "draw\n";

		w.swapBuffers();
	}

}

void RayTracing::rayTrace(){
	
	while (!w.isClosed()) {
		glfwPollEvents();
		std::cout << "trace\n";
		w.swapBuffers();
	}

}

void RayTracing::init(){

	std::ifstream is;

	
	// Compute shader - Ray Tracing
	std::string src = "";
	try {
		std::string tmp;
		is.open("../shaders/trace.cs");
		while (!(is.eof())) {
			std::getline(is, tmp);
			src.append(tmp);
			src.append("\n");
		}
		is.close();
	}
	catch (std::exception e) {
		is.close();
	}
	//std::cout << src << std::endl;

	auto cs = std::make_shared<ge::gl::Shader>(GL_COMPUTE_SHADER, src);
	tracer = std::make_shared<ge::gl::Program>(cs);
	// Compute shader - Ray Tracing


	// Vertex shader - display
	src = "";
	try {
		std::string tmp;
		is.open("../shaders/display.vs");
		while (!(is.eof())) {
			std::getline(is, tmp);
			src.append(tmp);
			src.append("\n");
		}
		is.close();
	}
	catch (std::exception e) {
		is.close();
	}

	auto vs = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, src);
	// Vertex shader - display


	// Fragment shader - display
	src = "";
	try {
		std::string tmp;
		is.open("../shaders/display.fs");
		while (!(is.eof())) {
			std::getline(is, tmp);
			src.append(tmp);
			src.append("\n");
		}
		is.close();
	}
	catch (std::exception e) {
		is.close();
	}
	auto fs = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, src);
	display = std::make_shared<ge::gl::Program>(vs, fs);
	// Fragment shader - display


	// SSBOs
	geomBuff = std::make_shared<ge::gl::Buffer>(sizeof(triangle) * s.getGeometry().size());
	matBuff = std::make_shared<ge::gl::Buffer>(sizeof(material) * s.getMaterials().size());
	renderBuff = std::make_shared<ge::gl::Buffer>(sizeof(glm::vec4) * w.getWidth() * w.getHeight());
	//renderTex = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA32F, 1, 1000, 800);
	ge::gl::glGenTextures(1, &renTex);
	ge::gl::glActiveTexture(GL_TEXTURE0);
	ge::gl::glBindTexture(GL_TEXTURE_2D, renTex);
	ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	ge::gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1000, 800, 0, GL_RGBA, GL_FLOAT, NULL);
	ge::gl::glBindImageTexture(0, renTex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
	
	// SSBOs

}
