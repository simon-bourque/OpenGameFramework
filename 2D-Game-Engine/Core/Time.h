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

inline string getSystemTimeString() {
	WCHAR wtimeStr[32];
	CHAR timeStr[32];
	WCHAR* format = L"'['HH':'mm':'ss']'";
	GetTimeFormatEx(LOCALE_NAME_USER_DEFAULT, TIME_FORCE24HOURFORMAT, NULL, format, wtimeStr, 32);
	WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, wtimeStr, -1, timeStr, 32 * sizeof(CHAR), NULL, NULL);

	return string(timeStr);
}

#endif

#ifdef USING_GLFW_WINDOW

#include <GLFW/glfw3.h>
#include <ctime>

inline float64 getCurrentTime() {
	return glfwGetTime();
}

inline string getSystemTimeString() {
	time_t currentTimeRaw = 0;
	struct tm currentTime;

	time(&currentTimeRaw);
	localtime_s(&currentTime, &currentTimeRaw);

	const static int32 dateStrSize = 128;
	char dateStr[dateStrSize];

	strftime(dateStr, dateStrSize, "[%T] ", &currentTime);

	return string(dateStr);
}

#endif