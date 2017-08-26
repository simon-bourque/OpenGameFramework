#pragma once


#ifdef _WIN32

// Windows specific defines
#define PLATFORM_WINDOWS

#define USING_WIN32_FILES
#define USING_WIN32_WINDOW
#define USING_WIN32_CONSOLE
//#define USING_GLFW_WINDOW

#else

#error Undefined platform.

#endif

#include <string>

void displayErrorBox(const std::string& msg);
void printSystemInfo();
