#include "Input.h"

#include "Window.h"

Input::Input(Window* window)
{
	glfwSetKeyCallback(window->m_handle, keyCallback);
	glfwSetCursorPosCallback(window->m_handle, cursorPositionCallback);
}


Input::~Input()
{
	for (KeyListenerBase* listener : m_keyListeners) {
		delete listener;
	}

	for (CursorPositionListenerBase* listener : m_cursorPositionListeners) {
		delete listener;
	}
}

static void keyCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods) {
	Window* windowPtr = (Window*)glfwGetWindowUserPointer(window);
	Input* input = windowPtr->getInput();


	for (Input::KeyListenerBase* listener: input->m_keyListeners) {
		(*listener)(key, scancode, action, mods);
	}
}

static void cursorPositionCallback(GLFWwindow* window, float64 xPos, float64 yPos) {
	Window* windowPtr = (Window*)glfwGetWindowUserPointer(window);
	Input* input = windowPtr->getInput();


	for (Input::CursorPositionListenerBase* listener : input->m_cursorPositionListeners) {
		(*listener)(xPos, yPos);
	}
}