/*

*/

#pragma once

#include <Camera.h>
#include <FPSCamera.h>

#include <GLFW/glfw3.h>

class FPSCameraManager {

public:

	FPSCameraManager(){
	}

	FPSCamera c;
	
	bool first = true;
	double prevX, prevY;
	bool pressed = false;

	float delta = 0.0f, last = 0.0f;

	void values_update(float currentTime);

	void mouse_callback(GLFWwindow* window);

	void key_callback(GLFWwindow* window);

	void camera_move(GLFWwindow* window, float currentTime);
};