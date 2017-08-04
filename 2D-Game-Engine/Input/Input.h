#pragma once

#include "Core/Core.h"
#include "Core/Delegate.h"
#include "Core/EngineAssert.h"

#include <vector>

#include "Input/InputConstants.h"

class Input
{
	
	friend void keyCallback(int32 key, int32 scancode, int32 action, int32 mods);
	friend void cursorPositionCallback(float64 xPos, float64 yPos);
	friend void mouseButtonCallback(int32 button, int32 action, int32 mods);
	friend void scrollCallback(float64 xOffset, float64 yOffset);
private:
	static Input* s_instance;

	typedef BaseDelegate<int32, int32, int32, int32> KeyListener;
	typedef BaseDelegate<float64, float64> CursorPositionListener, ScrollListener;
	typedef BaseDelegate<int32, int32, int32> MouseButtonListener;

	std::vector<KeyListener*> m_keyListeners;
	std::vector<CursorPositionListener*> m_cursorPositionListeners;
	std::vector<MouseButtonListener*> m_mouseButtonListeners;
	std::vector<ScrollListener*> m_scrollListeners;

	Input();

	void keyCallback(int32 key, int32 scancode, int32 action, int32 mods);
	void cursorPositionCallback(float64 xPos, float64 yPos);
	void mouseButtonCallback(int32 button, int32 action, int32 mods);
	void scrollCallback(float64 xOffset, float64 yOffset);
public:
	virtual ~Input();

	template <typename T>
	void addKeyListener(T* object, void(T::*method)(int32 key, int32 scancode, int32 action, int32 mods));

	template <typename T>
	void addCursorPositionListener(T* object, void(T::*method)(float64 xPos, float64 yPos));

	template <typename T>
	void addMouseButtonListener(T* object, void(T::*method)(int32 button, int32 action, int32 mods));

	template <typename T>
	void addScrollListener(T* object, void(T::*method)(float64 xOffset, float64 yOffset));

	static Input* get() {
		ASSERT(s_instance, "Input must be initialized before use.");
		return s_instance;
	};

	static void init() {
		ASSERT(!s_instance, "Input is already initialized.");
		s_instance = new Input();
	}

	static void destroy() {
		delete s_instance;
	}
};

template <typename T>
void Input::addKeyListener(T* object, void(T::*method)(int32 key, int32 scancode, int32 action, int32 mods)) {
	Delegate<T, int32, int32, int32, int32>* listener = new Delegate<T, int32, int32, int32, int32>(object, method);
	m_keyListeners.push_back(listener);
}

template <typename T>
void Input::addCursorPositionListener(T* object, void(T::*method)(float64 xPos, float64 yPos)) {
	Delegate<T, float64, float64>* listener = new Delegate<T, float64, float64>(object, method);
	m_cursorPositionListeners.push_back(listener);
}

template <typename T>
void Input::addMouseButtonListener(T* object, void(T::*method)(int32 button, int32 action, int32 mods)) {
	Delegate<T, int32, int32, int32>* listener = new Delegate<T, int32, int32, int32>(object, method);
	m_mouseButtonListeners.push_back(listener);
}

template <typename T>
void Input::addScrollListener(T* object, void(T::*method)(float64 xOffset, float64 yOffset)) {
	Delegate<T, float64, float64>* listener = new Delegate<T, float64, float64>(object, method);
	m_scrollListeners.push_back(listener);
}