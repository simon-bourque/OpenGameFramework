#pragma once

#include "Core/Core.h"
#include "Core/Platform.h"

#ifdef USING_WIN32_WINDOW

#include <Windows.h>

inline float64 getCurrentTime() {
	LARGE_INTEGER time, frequency;

	QueryPerformanceCounter(&time);
	QueryPerformanceFrequency(&frequency);

	return (time.QuadPart / (float64)frequency.QuadPart);
}

#endif

#ifdef USING_GLFW_WINDOW

#include <GLFW/glfw3.h>

inline float64 getCurrentTime() {
	return glfwGetTime();
}

#endif