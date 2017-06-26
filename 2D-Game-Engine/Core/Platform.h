#pragma once


#ifdef _WIN32

#define OS_WINDOWS

#endif

#include <string>

void displayErrorBox(const std::string& msg);
void printSystemInfo();
