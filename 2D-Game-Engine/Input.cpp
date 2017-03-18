#include "Input.h"

#include "Window.h"

Input::Input(Window* window)
{
	glfwSetKeyCallback(window->m_handle, keyCallback);
}


Input::~Input()
{
}

void Input::addKeyListener(void* object, void(*func)(void* object, int32 key, int32 scancode, int32 action, int32 mods)) {
	m_keyListeners.push_back({ object, func });
}

void keyCallback(GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods) {
	Window* windowPtr = (Window*)glfwGetWindowUserPointer(window);
	Input* input = (Input*)windowPtr->getInput();

	for (Input::KeyListener listener : input->m_keyListeners) {
		listener.func(listener.object, key, scancode, action, mods);
	}
}