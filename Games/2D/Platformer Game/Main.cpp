#include "PlatformerGame.h"

#include "Core/Core.h"
#include "Core/Platform.h"

#include "Core/EngineAssert.h"

#include <DbgHelp.h>

int main() {
	SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
	HANDLE process = GetCurrentProcess();
	if (!SymInitialize(process, NULL, TRUE))
	{
		// SymInitialize failed
		DWORD error = GetLastError();
		printf("SymInitialize returned error : %d\n", error);
		return 1;
	}

	ASSERT(false, "Test message");
	try {
		PlatformerGame game;
		game.start();
	}
	catch (std::runtime_error& ex) {
		DEBUG_LOG("Exception occured.");
		DEBUG_LOG(ex.what());
		displayErrorBox(ex.what());
		return 1;
	}
	//system("pause");

	if (SymCleanup(process))
	{
		// SymCleanup returned success
	}
	else
	{
		// SymCleanup failed
		DWORD error = GetLastError();
		printf("SymCleanup returned error : %d\n", error);
	}

	return 0;
}