#pragma once

#ifdef DEBUG_BUILD

#include "Core/Platform.h"
#include "Core/Core.h"

#ifdef PLATFORM_WINDOWS

#include <Windows.h>

bool showAssertDialogWindows(const string& msg, const string& fileName, uint32 lineNumber);

#define ASSERT(expr, msg)	do { \
								if (!expr) { \
									if (showAssertDialogWindows(msg, __FILE__, __LINE__)) { \
											DebugBreak(); \
									} \
								} \
							} while (0)
#else
	#define ASSERT(expr, msg) ((void)0)
#endif

#else
	#define ASSERT(expr, msg) ((void)0)
#endif