#pragma once

#include "Core/Core.h"
#include "Core/Delegate.h"
#include "Core/EngineAssert.h"

#include <vector>

#include "Input/InputConstants.h"

typedef Delegate<int32, int32, int32, int32> KeyListener;
typedef Delegate<float64, float64> CursorPositionListener, ScrollListener;
typedef Delegate<int32, int32, int32> MouseButtonListener;

class Input
{
	
	friend void keyCallback(int32 key, int32 scancode, int32 action, int32 mods);
	friend void cursorPositionCallback(float64 xPos, float64 yPos);
	friend void mouseButtonCallback(int32 button, int32 action, int32 mods);
	friend void scrollCallback(float64 xOffset, float64 yOffset);
private:
	static Input* s_instance;

	std::vector<KeyListener> m_keyListeners;
	std::vector<CursorPositionListener> m_cursorPositionListeners;
	std::vector<MouseButtonListener> m_mouseButtonListeners;
	std::vector<ScrollListener> m_scrollListeners;

	Input();

	void keyCallback(int32 key, int32 scancode, int32 action, int32 mods);
	void cursorPositionCallback(float64 xPos, float64 yPos);
	void mouseButtonCallback(int32 button, int32 action, int32 mods);
	void scrollCallback(float64 xOffset, float64 yOffset);
public:
	virtual ~Input();

	void addKeyListener(const Delegate<int32, int32, int32, int32>& keyListener);
	void addCursorPositionListener(const Delegate<float64, float64>& cursorPositionListener);
	void addMouseButtonListener(const Delegate<int32, int32, int32>& mouseButtonListener);
	void addScrollListener(const Delegate<float64, float64>& scrollListener);

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