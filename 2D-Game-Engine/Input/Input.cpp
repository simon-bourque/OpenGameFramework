#include "Input.h"

#include "Graphics/Window.h"

Input::Input(Window* window)
{
	glfwSetKeyCallback(window->m_handle, keyCallback);
	glfwSetCursorPosCallback(window->m_handle, cursorPositionCallback);
	glfwSetMouseButtonCallback(window->m_handle, mouseButtonCallback);
	glfwSetScrollCallback(window->m_handle, scrollCallback);
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
	Input* input = ((Window*)glfwGetWindowUserPointer(window))->getInput();

	for (Input::KeyListener* listener: input->m_keyListeners) {
		(*listener)(key, scancode, action, mods);
	}
}

static void cursorPositionCallback(GLFWwindow* window, float64 xPos, float64 yPos) {
	Input* input = ((Window*)glfwGetWindowUserPointer(window))->getInput();

	for (Input::CursorPositionListener* listener : input->m_cursorPositionListeners) {
		(*listener)(xPos, yPos);
	}
}

static void mouseButtonCallback(GLFWwindow* window, int32 button, int32 action, int32 mods) {
	Input* input = ((Window*)glfwGetWindowUserPointer(window))->getInput();

	for (Input::MouseButtonListener* listener : input->m_mouseButtonListeners) {
		(*listener)(button, action, mods);
	}
}

static void scrollCallback(GLFWwindow* window, float64 xOffset, float64 yOffset) {
	Input* input = ((Window*)glfwGetWindowUserPointer(window))->getInput();

	for (Input::ScrollListener* listener : input->m_scrollListeners) {
		(*listener)(xOffset, yOffset);
	}
}