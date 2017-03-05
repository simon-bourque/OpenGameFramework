#ifndef CORE_H
#define CORE_H

#include <string>

#include "Types.h"
#include "Logging.h"

#ifdef DEBUG_BUILD
	#define DEBUG_LOG(msg) printToConsole(msg)
#else
	#define DEBUG_LOG(msg)
#endif

using namespace std;

#endif