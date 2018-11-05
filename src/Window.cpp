#include "Window.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

Window::Window(int width, int height, std::string title, bool createImmediately) {
	this->width = width;
	this->height = height;
	this->title = title;

	if (createImmediately)
		setDefaults(), createWindow();

}


Window::~Window() {
	destroyGUI();
	glfwDestroyWindow(window.get());
	glfwTerminate();
	window.release();
}


bool Window::isClosed() {
	if (glfwWindowShouldClose(window.get()))
		return true;

	glfwPollEvents();
	return false;
}


void Window::swapBuffers() {
	glfwSwapBuffers(window.get());
}


GLFWwindow* Window::getWindow() {
	return window.get();
}

void Window::initGUI(){
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("DroidSans.ttf", 14.0f);

	ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
}

void Window::renderGUI(){
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Rendering counter");
	char v;
	ImGui::ColorButton(&v, ImVec4());

	ImGui::NewLine();
	ImGui::Text("Raytrace app");
	
	ImGui::NewLine();

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::destroyGUI(){
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}


int Window::getWidth() {
	return width;
}


int Window::getHeight() {
	return height;
}


void Window::makeContextCurrent() {
	glfwMakeContextCurrent(window.get());
}


void Window::setWindowProperty(int property, int value) {
	glfwWindowHint(property, value);
}


void Window::setDefaults() {
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 16);
}


bool Window::createWindow() {

	if (!glfwInit())
		return false;

	window.reset(glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr));

	if (window.get() == nullptr)
		return false;

	glfwMakeContextCurrent(window.get());

	ge::gl::init();

	int w, h;
	glfwGetFramebufferSize(window.get(), &w, &h);
	ge::gl::glViewport(0, 0, w, h);

	return true;
}


void Window::enableFeature(int feature) {
	ge::gl::glEnable(feature);
}


void Window::setKeyCallback(GLFWkeyfun function) {
	glfwSetKeyCallback(window.get(), function);
}


void Window::setMouseCallback(GLFWcursorposfun function) {
	glfwSetCursorPosCallback(window.get(), function);
}


void Window::setCursorCallback(GLFWcursorposfun function) {
	glfwSetCursorPosCallback(window.get(), function);
}


void Window::setResizeCallback(GLFWwindowsizefun function) {
	glfwSetWindowSizeCallback(window.get(), function);
}


void Window::setWindowRefreshCallback(GLFWwindowrefreshfun function) {
	glfwSetWindowRefreshCallback(window.get(), function);
}


void Window::setScrollCallback(GLFWscrollfun function) {
	glfwSetScrollCallback(window.get(), function);
}