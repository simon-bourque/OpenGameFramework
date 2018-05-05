#include "PlatformerGame.h"

#include "Core/Core.h"
#include "Core/Platform.h"

#include "Core/Assert.h"

#include "Core/Debug/Debug.h"

int main() {
	Debug::loadSymbols();

	OGF_ASSERT(false, "%s:%f Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",  "THE NUMBER IS", 1.5f);
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

	Debug::unloadSymbols();

	return 0;
}