#pragma once

#include "Core/Core.h"

#include "Core/EngineAssert.h"

#include "Core/Platform.h"

#ifdef USING_WIN32_WINDOW
	#include "Windows.h"
#endif


class Window {
private:
	static Window* s_instance;

	string m_title;
	int32 m_width;
	int32 m_height;

	bool m_shouldClose;

#ifdef USING_WIN32_WINDOW
	HWND m_handle;
	HDC m_deviceContext;
	HGLRC m_openglRenderContext;

	friend LRESULT CALLBACK MainWndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

	Window(const string& title, int32 width, int32 height);
public:
	virtual ~Window();

	int32 getWidth() const { return m_width; };
	int32 getHeight() const { return m_height; };

	void setTitle(const string& title);
	void swapBuffers() const;
	bool shouldClose() const;

	void pollEvents();

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

	static bool isInitialized() {
		return (s_instance != nullptr);
	}

	static void destroy() {
		delete s_instance;
	}
};