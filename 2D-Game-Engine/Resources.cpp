
#include "Core.h"

#include "RawImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <sstream>
#include <fstream>

string loadSrc(string file) {
	ifstream input(file);

	if (!input) {
		input.close();
		throw runtime_error("Failed to load text from file.");
	}

	stringstream ss;

	while (!input.eof()) {
		int8 character = input.get();

		if (input.eof()) {
			break;
		}

		ss << character;
	}

	input.close();

	return ss.str();
}

RawImage loadImage(string file) {
	int32 width = 0;
	int32 height = 0;
	int32 channels = 0;
	uint8* data = stbi_load(file.c_str(), &width, &height, &channels, 4);
	
	return RawImage(data, width, height, channels);
}