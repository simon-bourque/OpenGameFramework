#include "Input.h"

Input* Input::s_instance = nullptr;

Input::Input() {}

Input::~Input() {}

void Input::keyCallback(int32 key, int32 scancode, int32 action, int32 mods) {
	for (KeyListener& listener : m_keyListeners) {
		listener(key, scancode, action, mods);
	}
}

void Input::cursorPositionCallback(float64 xPos, float64 yPos) {
	for (CursorPositionListener& listener : m_cursorPositionListeners) {
		listener(xPos, yPos);
	}
}

void Input::mouseButtonCallback(int32 button, int32 action, int32 mods) {
	for (MouseButtonListener& listener : m_mouseButtonListeners) {
		listener(button, action, mods);
	}
}

void Input::scrollCallback(float64 xOffset, float64 yOffset) {
	for (ScrollListener& listener : m_scrollListeners) {
		listener(xOffset, yOffset);
	}
}

void Input::addKeyListener(const Delegate<int32, int32, int32, int32>& keyListener) {
	m_keyListeners.push_back(keyListener);
}

void Input::addCursorPositionListener(const Delegate<float64, float64>& cursorPositionListener) {
	m_cursorPositionListeners.push_back(cursorPositionListener);
}

void Input::addMouseButtonListener(const Delegate<int32, int32, int32>& mouseButtonListener) {
	m_mouseButtonListeners.push_back(mouseButtonListener);
}

void Input::addScrollListener(const Delegate<float64, float64>& scrollListener) {
	m_scrollListeners.push_back(scrollListener);
}