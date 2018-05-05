#include "PlatformerGame.h"

#include "Core/Core.h"
#include "Core/Platform.h"

#include "Core/Assert.h"

#include "Core/Debug/Debug.h"

int main() {
	Debug::loadSymbols();

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