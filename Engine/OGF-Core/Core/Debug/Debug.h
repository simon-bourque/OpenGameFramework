#pragma once


#include "Core/Core.h"

#include <vector>

class Debug {
private:
#ifdef DEBUG_BUILD
	static bool s_symbolsLoaded;
#endif
public:
#ifdef DEBUG_BUILD
	static bool loadSymbols();
	static bool unloadSymbols();

	static bool isDebuggerAttached();

	static bool getStackTrace(std::vector<string>& stackTrace);
	
	static string getSymbolFromAddress(uint64 address);
	static string getModuleFromAddress(uint64 address);
	static uint32 getLineNumberFromAddress(uint64 address);
#endif // DEBUG_BUILD

	static string getStringFromErrorCode(uint32 errorCode);
};