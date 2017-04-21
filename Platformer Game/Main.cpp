#include "PlatformerGame.h"

#include "Core.h"
#include <iostream>

#include "Resources.h"

#include "Vector2f.h"
#include "Matrix3f.h"

#include "Windows.h"

int main() {
	try {
		PlatformerGame game;
		game.run();
	}
	catch (runtime_error& ex) {
		DEBUG_LOG("Exception occured.");
		DEBUG_LOG(ex.what());
		//system("pause");
		return 1;
	}

	//system("pause");

	return 0;
}