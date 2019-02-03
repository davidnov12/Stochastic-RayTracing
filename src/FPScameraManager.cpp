#include <FPScameraManager.h>

void FPSCameraManager::values_update(float currentTime){

	//printf("%f\n", currentTime);

	delta = currentTime - last;
	last = currentTime;

	//printf("%f\n", c.position.x);
}

void FPSCameraManager::mouse_callback(GLFWwindow * window){

	double x, y;
	double xpos, ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) pressed = false;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) pressed = true;

	glfwGetCursorPos(window, &xpos, &ypos);

	if (first) {
		first = false;
		prevX = xpos;
		prevY = ypos;
	}

	x = xpos - prevX;
	y = prevY - ypos;

	prevX = xpos;
	prevY = ypos;

	if (pressed) {
		c.mouseEvent(x, y);
	}

}

void FPSCameraManager::key_callback(GLFWwindow * window){

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		c.keyboardEvent(Camera::FORWARD, delta);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		c.keyboardEvent(Camera::BACKWARD, delta);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		c.keyboardEvent(Camera::LEFT, delta);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		c.keyboardEvent(Camera::RIGHT, delta);

}

void FPSCameraManager::camera_move(GLFWwindow * window, float currentTime){

	values_update(currentTime);
	key_callback(window);
	mouse_callback(window);

}
