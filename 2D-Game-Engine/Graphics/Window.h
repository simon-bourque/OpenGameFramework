#pragma once

#include "Core/Core.h"

#include "Core/EngineAssert.h"

struct GLFWwindow;
class Input;

class Window
{
	friend Input;
	friend void windowSizeCallback(GLFWwindow* window, int32 width, int32 height);

private:
	static Window* s_instance;

	string m_title;
	int32 m_width;
	int32 m_height;

	GLFWwindow* m_handle;

	Window(const string& title, int32 width, int32 height);
public:
	virtual ~Window();

	int32 getWidth() const { return m_width; };
	int32 getHeight() const { return m_height; };

	void setTitle(const string& title);
	void swapBuffers() const;
	int32 shouldClose() const;

	static void pollEvents();

	// Prevent copying of window
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	static Window* get() {
		ASSERT(s_instance, "Window must be initialized before use.");
		return s_instance;
	};

	static void init(const string& title, int32 width, int32 height) {
		ASSERT(!s_instance, "Window is already initialized.");
		s_instance = new Window(title, width, height);
	}

	static void destroy() {
		delete s_instance;
	}
};