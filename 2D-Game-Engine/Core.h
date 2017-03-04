#ifndef CORE_H
#define CORE_H

#include <string>

#include "Types.h"
#include "Log.h"

#ifdef DEBUG_BUILD
	#define DEBUG_LOG(msg) log(msg)
#else
	#define DEBUG_LOG(msg)
#endif

using namespace std;

#endif