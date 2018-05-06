#include "Debug.h"

#include <Windows.h>

#ifdef DEBUG_BUILD

#include <sstream>
#include <DbgHelp.h>

bool Debug::s_symbolsLoaded = false;

bool Debug::loadSymbols() {
	if (s_symbolsLoaded) {
		return false;
	}
	
	SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);
	HANDLE process = GetCurrentProcess();
	if (!SymInitialize(process, NULL, TRUE))
	{
		// SymInitialize failed
		DWORD error = GetLastError();
		printf("SymInitialize returned error : %d\n", error);
		return false;
	}

	s_symbolsLoaded = true;
	return true;
}

bool Debug::unloadSymbols() {
	if (!s_symbolsLoaded) {
		return false;
	}

	HANDLE process = GetCurrentProcess();
	if (SymCleanup(process))
	{
		s_symbolsLoaded = false;
		return true;
	}
	else
	{
		// SymCleanup failed
		DWORD error = GetLastError();
		printf("SymCleanup returned error : %d\n", error);
		return false;
	}
}

bool Debug::isDebuggerAttached() {
	return IsDebuggerPresent() != 0;
}

bool Debug::getStackTrace(std::vector<string>& stackTrace) {
	if (!s_symbolsLoaded) {
		stackTrace.push_back(string("Failed to get stack trace, no symbols loaded."));
		return false;
	}
	
	CONTEXT context = { 0 };
	context.ContextFlags = CONTEXT_CONTROL;

#ifdef _WIN64
	RtlCaptureContext(&context);
#else
	__asm {
		LABEL:
		mov[context.Ebp], ebp;
		mov[context.Esp], esp;
		mov eax, [LABEL];
		mov[context.Eip], eax;
	}
#endif

	STACKFRAME64 stackFrame;
#ifdef _WIN64
#define MACHINE_TYPE IMAGE_FILE_MACHINE_AMD64
	stackFrame.AddrPC.Offset = context.Rip;
	stackFrame.AddrPC.Mode = AddrModeFlat;
	stackFrame.AddrFrame.Offset = context.Rbp;
	stackFrame.AddrFrame.Mode = AddrModeFlat;
	stackFrame.AddrStack.Offset = context.Rsp;
	stackFrame.AddrStack.Mode = AddrModeFlat;
#else
#define MACHINE_TYPE IMAGE_FILE_MACHINE_I386
	stackFrame.AddrPC.Offset = context.Eip;
	stackFrame.AddrPC.Mode = AddrModeFlat;
	stackFrame.AddrFrame.Offset = context.Ebp;
	stackFrame.AddrFrame.Mode = AddrModeFlat;
	stackFrame.AddrStack.Offset = context.Esp;
	stackFrame.AddrStack.Mode = AddrModeFlat;
#endif

	while (StackWalk64(MACHINE_TYPE, GetCurrentProcess(), GetCurrentThread(), &stackFrame, &context, NULL, SymFunctionTableAccess64, SymGetModuleBase64, NULL)) {
		uint64 address = stackFrame.AddrPC.Offset;
		std::stringstream symbolNameStream;

		symbolNameStream << getModuleFromAddress(address) << '!' << getSymbolFromAddress(address) << '(' << getLineNumberFromAddress(address) << ')';

		stackTrace.push_back(symbolNameStream.str());
	}

#undef MACHINE_TYPE
	return true;
}

string Debug::getSymbolFromAddress(uint64 address) {
	if (!s_symbolsLoaded) {
		return string("<unknown symbol>");
	}
	
	char buffer[sizeof(SYMBOL_INFO) + (MAX_SYM_NAME * sizeof(TCHAR))];
	PSYMBOL_INFO symInfo = reinterpret_cast<PSYMBOL_INFO>(buffer);
	symInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
	symInfo->MaxNameLen = MAX_SYM_NAME;

	if (SymFromAddr(GetCurrentProcess(), address, 0, symInfo)) {
		return string(symInfo->Name, symInfo->NameLen);
	}
	else {
		return string("<unknown symbol>");
	}
}

string Debug::getModuleFromAddress(uint64 address) {
	if (!s_symbolsLoaded) {
		return string("<unknown module>");
	}

	IMAGEHLP_MODULE64 moduleInfo;
	moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);

	if (SymGetModuleInfo64(GetCurrentProcess(), address, &moduleInfo)) {
		return string(moduleInfo.ModuleName);
	}
	else {
		return string("<unknown module>");
	}
}

uint32 Debug::getLineNumberFromAddress(uint64 address) {
	if (!s_symbolsLoaded) {
		return 0;
	}

	IMAGEHLP_LINE64 lineInfo;
	lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

	DWORD displacement = 0;
	if (SymGetLineFromAddr64(GetCurrentProcess(), address, &displacement, &lineInfo)) {
		return lineInfo.LineNumber;
	}
	else {
		return 0;
	}
}
#endif

string Debug::getStringFromErrorCode(uint32 errorCode) {
	
	char* messageBuffer = nullptr;
	
	uint32 numChars = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&messageBuffer,
		0, NULL);

	string message = (numChars != 0) ? string(messageBuffer, numChars) : std::to_string(errorCode);
	LocalFree(messageBuffer);

	return message;
}
