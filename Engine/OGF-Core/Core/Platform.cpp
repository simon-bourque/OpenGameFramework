#include "Platform.h"

#ifdef PLATFORM_WINDOWS
	#include "Windows.h"
#endif

#include "Core/Core.h"

void displayErrorBox(const std::string& msg) {
#ifdef PLATFORM_WINDOWS
	MessageBox(NULL, msg.c_str(), "Exception", MB_OK | MB_ICONERROR);
#endif
}

void printSystemInfo() {
#ifdef PLATFORM_WINDOWS
	// Documentation for gathering system info on Windows https://msdn.microsoft.com/en-us/library/windows/desktop/ms724951.aspx

	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);

	DEBUG_LOG("-------------------- System Info --------------------");

	// Processor
	DEBUG_LOG("Processor:");
	if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
		DEBUG_LOG("\tArchitecture: x64");
	}
	else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
		DEBUG_LOG("\tArchitecture: x86");
	}
	else {
		DEBUG_LOG("\tArchitecture: Other");
	}

	DEBUG_LOG("\tLogical Processors: " + std::to_string(sysInfo.dwNumberOfProcessors));

	DEBUG_LOG("-----------------------------------------------------");
#endif
}
