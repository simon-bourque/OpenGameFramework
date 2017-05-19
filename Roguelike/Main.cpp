#include "Core/Core.h"
#include "Core/Platform.h"

#include "RoguelikeGame.h"

int main() {
	//ASSERT(false, "Test message");
	try {
		RoguelikeGame game;
		game.run();
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