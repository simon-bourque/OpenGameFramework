#ifndef ENGINEASSERT_H
#define ENGINEASSERT_H

#ifdef DEBUG_BUILD

#include "Platform.h"
#include "Core.h"

#ifdef OS_WINDOWS

void debugBreakWindows();
bool showAssertDialogWindows(const string& msg, const string& fileName, uint32 lineNumber);

#define ASSERT(expr, msg)	if (!expr) { \
								if (showAssertDialogWindows(msg, __FILE__, __LINE__)) { \
									debugBreakWindows(); \
								} \
							}
#else
	#define ASSERT(expr, msg) ((void)0)
#endif

#else
	#define ASSERT(expr, msg) ((void)0)
#endif

#endif