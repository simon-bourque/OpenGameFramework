#pragma once

#ifdef DEBUG_BUILD

#include "Core/Platform.h"
#include "Core/Core.h"
#include "Core/Debug/Debug.h"

#ifdef PLATFORM_WINDOWS

#include <vector>
#include <Windows.h>

bool showAssertDialogWindows(const std::wstring& msg);
std::wstring formatAssertMessage(const char* fileName, uint32 lineNumber, const std::vector<string>& stackTrace, const char* msg, ...);

#define OGF_ASSERT(expr, msg, ...)	do { \
								if (!expr) { \
									std::vector<string> stackTrace; \
									Debug::getStackTrace(stackTrace); \
									if (showAssertDialogWindows(formatAssertMessage(__FILE__, __LINE__, stackTrace, msg, ##__VA_ARGS__))) { \
											DebugBreak(); \
									} \
								} \
							} while (0)
#else
	#define OGF_ASSERT(expr, msg, ...) ((void)0)
#endif

#else
	#define OGF_ASSERT(expr, msg, ...) ((void)0)
#endif