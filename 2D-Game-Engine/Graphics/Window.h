#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include "Core/Core.h"

struct GLFWwindow;
class Input;

class Window
{
	friend Input;
	friend void windowSizeCallback(GLFWwindow* window, int32 width, int32 height);

private:
	string m_title;
	int32 m_width;
	int32 m_height;

	GLFWwindow* m_handle;

	Input* m_input;
public:
	Window(string title, int32 width, int32 height);
	virtual ~Window();

	int32 getWidth() const { return m_width; };
	int32 getHeight() const { return m_height; };
	Input* getInput() const { return m_input; };

	void setTitle(const string& title);
	void swapBuffers() const;
	int32 shouldClose() const;

	static void pollEvents();

	// Prevent copying of window
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
};

#endif

