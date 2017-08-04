#include "Window.h"

#ifdef USING_GLFW_WINDOW

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Input/Input.h"

#include <exception>

Window* Window::s_instance = nullptr;

Window::Window(const string& title, int32 width, int32 height) : m_title(title), m_width(width), m_height(height) {

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

	// Set callbacks

	glfwSetWindowSizeCallback(m_handle, windowSizeCallback);

	glfwSetKeyCallback(m_handle, [](GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods) -> void { keyCallback(key, scancode, action, mods); });
	glfwSetCursorPosCallback(m_handle, [](GLFWwindow* window, float64 xPos, float64 yPos) -> void { cursorPositionCallback(xPos, yPos); });
	glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* window, int32 button, int32 action, int32 mods) -> void { mouseButtonCallback(button, action, mods); });
	glfwSetScrollCallback(m_handle, [](GLFWwindow* window, float64 xOffset, float64 yOffset) -> void { scrollCallback(xOffset, yOffset); });
}


Window::~Window() {
	glfwTerminate();
}

static void windowSizeCallback(GLFWwindow* window, int32 width, int32 height) {
	Window* windowPtr = Window::get();
	windowPtr->m_width = width;
	windowPtr->m_height = height;
	glViewport(0, 0, width, height);
}

static void keyCallback(int32 key, int32 scancode, int32 action, int32 mods) {
	Input::get()->keyCallback(key, scancode, action, mods);
}

static void cursorPositionCallback(float64 xPos, float64 yPos) {
	Input::get()->cursorPositionCallback(xPos, yPos);
}

static void mouseButtonCallback(int32 button, int32 action, int32 mods) {
	Input::get()->mouseButtonCallback(button, action, mods);
}

static void scrollCallback(float64 xOffset, float64 yOffset) {
	Input::get()->scrollCallback(xOffset, yOffset);
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

#endif