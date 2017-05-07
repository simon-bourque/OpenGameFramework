#pragma once
#ifndef PLATFORM_H
#define PLATFORM_H


#ifdef _WIN32

#define OS_WINDOWS

#endif

#include <string>

void displayErrorBox(const std::string& msg);

#endif
