#pragma once
#ifndef CORE_H
#define CORE_H

#include "Core/Types.h"
#include "Core/Logging.h"

#include <string>
using std::string;

#ifdef DEBUG_BUILD
	#define DEBUG_LOG(msg) printToConsole(msg)
#else
	#define DEBUG_LOG(msg) ((void)0)
#endif

#endif