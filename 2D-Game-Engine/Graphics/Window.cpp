#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Input/Input.h"

#include <exception>

Window::Window(string title, int32 width, int32 height) : m_title(title), m_width(width), m_height(height) {

	glfwSetErrorCallback([](int32 error, const char* desc) -> void { DEBUG_LOG(desc); });

	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW.");
	}

	glfwDefaultWindowHints();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!m_handle)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to initialize window.");
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

	m_input = new Input(this);
}


Window::~Window() {
	delete m_input;
	glfwTerminate();
}

static void windowSizeCallback(GLFWwindow* window, int32 width, int32 height) {
	Window* windowPtr = (Window*)glfwGetWindowUserPointer(window);
	windowPtr->m_width = width;
	windowPtr->m_height = height;
	glViewport(0, 0, width, height);
}

void Window::setTitle(const string& title) {
	this->m_title = title;
	glfwSetWindowTitle(m_handle, title.c_str());
}

void Window::swapBuffers() const {
	glfwSwapBuffers(m_handle);
}

int32 Window::shouldClose() const {
	return glfwWindowShouldClose(m_handle);
}

void Window::pollEvents() {
	glfwPollEvents();
}