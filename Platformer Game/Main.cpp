#include "PlatformerGame.h"

#include "Core/Core.h"
#include "Core/Platform.h"

#include "Resource/File/FileReader.h"

#include <iostream>

#include <chrono>
#include <fstream>

int main() {
	//ASSERT(false, "Test message");
	try {
		char v = 0;
		auto t1 = std::chrono::high_resolution_clock::now();
		FileReader input("test.txt");

		//std::cout << input.getFileSize() << std::endl;
		while (!input.isEndOfFile()) {
			v = input.readUInt8();
		}
		//std::cout << std::endl << std::to_string(input.readUInt8());
		input.close();
		//std::cout << std::to_string(input.readUInt8()) << std::endl;
		std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - t1).count() << " ns" << std::endl;

		t1 = std::chrono::high_resolution_clock::now();
		std::ifstream input2("test.txt");

		while (!input2.eof()) {
			v = input2.get();
		}
		input2.close();
		std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - t1).count() << " ns" << std::endl;

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