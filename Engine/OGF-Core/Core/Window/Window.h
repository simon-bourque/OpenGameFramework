#pragma once

#include "Core/Core.h"

#include "Core/Assert.h"

#include "Core/Platform.h"

#include "Core/Singleton.h"

#ifdef USING_WIN32_WINDOW
	#include "Windows.h"
#endif


class Window {
	SINGLETON_DECLARATION(Window)
private:
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
};

SINGLETON_ACCESSOR(Window);