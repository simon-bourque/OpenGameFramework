#ifndef ENGINEASSERT_H
#define ENGINEASSERT_H

#ifdef DEBUG_BUILD

#include "Platform.h"

#ifdef OS_WINDOWS
	#include "Windows.h"
	#define ASSERT(expr, msg)	if (!expr) { \
									DebugBreak(); \
								}
#else
	#define ASSERT(expr, msg) ((void)0)
#endif

#else
	#define ASSERT(expr, msg) ((void)0)
#endif

#endif