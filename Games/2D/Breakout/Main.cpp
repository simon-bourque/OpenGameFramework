#include "BreakoutGame.h"

#include "Core/Core.h"
#include "Core/Platform.h"

#include "Core/Assert.h"

#include "Core/Debug/Debug.h"

int main() {
#ifdef DEBUG_BUILD
	Debug::loadSymbols();
#endif

	try {
		BreakoutGame game;
		game.start();
	}
	catch (std::runtime_error& ex) {
		DEBUG_LOG("Exception occured.");
		DEBUG_LOG(ex.what());
		displayErrorBox(ex.what());
		return 1;
	}

#ifdef DEBUG_BUILD
	Debug::unloadSymbols();
#endif

	return 0;
}