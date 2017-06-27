#pragma once


#ifdef _WIN32

#define PLATFORM_WINDOWS

#else

#error Unkown platform.

#endif

#include <string>

void displayErrorBox(const std::string& msg);
void printSystemInfo();
