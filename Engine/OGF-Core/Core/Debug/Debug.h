#pragma once

#include "Core/Core.h"

#include <vector>

class Debug {
private:
	static bool s_symbolsLoaded;
public:
	static bool loadSymbols();
	static bool unloadSymbols();

	static bool isDebuggerAttached();

	static bool getStackTrace(std::vector<string>& stackTrace);
	
	static string getSymbolFromAddress(uint64 address);
	static string getModuleFromAddress(uint64 address);
	static uint32 getLineNumberFromAddress(uint64 address);

	static string getStringFromErrorCode(uint32 errorCode);
};