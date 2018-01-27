#include "PlatformerGame.h"

#include "Core/Core.h"
#include "Core/Platform.h"

//#include "Core/EngineAssert.h"

int main() {
	//ASSERT(false, "Test message");
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

	return 0;
}