#include "Input.h"

#include "Graphics/Window.h"

Input* Input::s_instance = nullptr;

Input::Input()
{
	glfwSetKeyCallback(Window::get()->m_handle, keyCallback);
	glfwSetCursorPosCallback(Window::get()->m_handle, cursorPositionCallback);
	glfwSetMouseButtonCallback(Window::get()->m_handle, mouseButtonCallback);
	glfwSetScrollCallback(Window::get()->m_handle, scrollCallback);
}

Input::~Input()
{
	for (KeyListener* listener : m_keyListeners) {
		delete listener;
	}

	for (CursorPositionListener* listener : m_cursorPositionListeners) {
		delete listener;
	}
	
	for (MouseButtonListener* listener : m_mouseButtonListeners) {
		delete listener;
	}

	for (ScrollListener* listener : m_scrollListeners) {
		delete listener;
	}
}

static void keyCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods) {
	Input* input = Input::get();

	for (Input::KeyListener* listener: input->m_keyListeners) {
		(*listener)(key, scancode, action, mods);
	}
}

static void cursorPositionCallback(GLFWwindow* window, float64 xPos, float64 yPos) {
	Input* input = Input::get();

	for (Input::CursorPositionListener* listener : input->m_cursorPositionListeners) {
		(*listener)(xPos, yPos);
	}
}

static void mouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods) {
	Input* input = Input::get();

	for (Input::MouseButtonListener* listener : input->m_mouseButtonListeners) {
		(*listener)(button, action, mods);
	}
}

static void scrollCallback(GLFWwindow* window, float64 xOffset, float64 yOffset) {
	Input* input = Input::get();

	for (Input::ScrollListener* listener : input->m_scrollListeners) {
		(*listener)(xOffset, yOffset);
	}
}