#include "Window.h"

#include <GLFW/glfw3.h>
#include <exception>

// Temp include
#include <iostream>

Window::Window(string title, int32 width, int32 height) : m_title(title), m_width(width), m_height(height) {

	glfwSetErrorCallback([](int32 error, const char* desc) -> void { cout << desc << endl; });

	if (!glfwInit()) {
		throw runtime_error("Failed to initialize GLFW.");
	}

	m_handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!m_handle)
	{
		glfwTerminate();
		throw runtime_error("Failed to initialize window.");
	}

	glfwMakeContextCurrent(m_handle);
	glfwSwapInterval(1);

	// Center window
	const GLFWvidmode* vid = glfwGetVideoMode(glfwGetPrimaryMonitor());
	int32 x = (vid->width / 2) - (width / 2);
	int32 y = (vid->height / 2) - (height / 2);
	glfwSetWindowPos(m_handle, x, y);

	glfwShowWindow(m_handle);

	glfwSetWindowUserPointer(m_handle, this);
	glfwSetWindowSizeCallback(m_handle, windowSizeCallback);
}


Window::~Window() {
	glfwTerminate();
}

void windowSizeCallback(GLFWwindow* window, int32 width, int32 height) {
	Window* windowPtr = (Window*)glfwGetWindowUserPointer(window);
	windowPtr->m_width = width;
	windowPtr->m_height = height;
}

void Window::setTitle(const string& title) {
	this->m_title = title;
	glfwSetWindowTitle(m_handle, title.c_str());
}

void Window::swapBuffers() const {
	glfwSwapBuffers(m_handle);
}

bool Window::shouldClose() const {
	return glfwWindowShouldClose(m_handle);
}

void Window::pollEvents() {
	glfwPollEvents();
}