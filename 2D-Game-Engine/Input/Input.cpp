#include "Input.h"

Input* Input::s_instance = nullptr;

Input::Input() {}

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

void Input::keyCallback(int32 key, int32 scancode, int32 action, int32 mods) {
	for (Input::KeyListener* listener : m_keyListeners) {
		(*listener)(key, scancode, action, mods);
	}
}

void Input::cursorPositionCallback(float64 xPos, float64 yPos) {
	for (Input::CursorPositionListener* listener : m_cursorPositionListeners) {
		(*listener)(xPos, yPos);
	}
}

void Input::mouseButtonCallback(int32 button, int32 action, int32 mods) {
	for (Input::MouseButtonListener* listener : m_mouseButtonListeners) {
		(*listener)(button, action, mods);
	}
}

void Input::scrollCallback(float64 xOffset, float64 yOffset) {
	for (Input::ScrollListener* listener : m_scrollListeners) {
		(*listener)(xOffset, yOffset);
	}
}