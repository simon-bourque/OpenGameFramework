#include "PlatformerGame.h"

#include "Core/Core.h"
#include "Core/Platform.h"

#include "Resource/File/FileReader.h"

#include <iostream>

int main() {
	//ASSERT(false, "Test message");
	try {
		uint8 v = 0;
		FileReader input("test.txt");

		std::cout << input.getFileSize() << std::endl;
		while (!input.isEndOfFile()) {
			std::cout << input.readUInt8();
		}
		std::cout << std::endl << std::to_string(input.readUInt8());
		input.close();
		std::cout << std::to_string(input.readUInt8()) << std::endl;

		PlatformerGame game;
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