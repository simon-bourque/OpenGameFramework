
#include "Core.h"

#include "RawImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <sstream>
#include <fstream>

void readFloat(ifstream& input, float32& value);
void readInt(ifstream& input, int32& value);

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

// Temp return a string representing the level
string loadLevel(string file) {
	ifstream input(file);

	if (!input) {
		input.close();
		throw runtime_error("Failed to load level.");
	}

	stringstream ss;

	// Load level bounds
	float32 boundsX = 0;
	float32 boundsY = 0;
	int32 boundsWidth = 0;
	int32 boundsHeight = 0;
	readFloat(input, boundsX);
	readFloat(input, boundsY);
	readInt(input, boundsWidth);
	readInt(input, boundsHeight);

	ss << boundsX << ' ';
	ss << boundsY << ' ';
	ss << boundsWidth << ' ';
	ss << boundsHeight << endl;

	int32 numTiles = 0;
	readInt(input, numTiles);

	ss << "TILES: " << numTiles << endl;

	for (uint32 i = 0; i < numTiles; i++) {
		float32 x = 0;
		float32 y = 0;
		int32 index = 0;
		readFloat(input, x);
		readFloat(input, y);
		readInt(input, index);
		ss << "{" << x << ", " << y << ", " << index << "}" << endl;
	}

	int32 numColliders = 0;
	readInt(input, numColliders);
	
	ss << "COLLIDERS: " << numColliders << endl;

	for (uint32 i = 0; i < numColliders; i++) {
		float32 x = 0;
		float32 y = 0;
		float32 width = 0;
		float32 height = 0;
		readFloat(input, x);
		readFloat(input, y);
		readFloat(input, width);
		readFloat(input, height);
		ss << "{" << x << ", " << y << ", " << width << ", " << height << "}" << endl;
	}

	int32 stringLen = 0;
	readInt(input, stringLen);

	for (uint32 i = 0; i < stringLen; i++) {
		char character = 0;
		input.get(character);
		ss << character;
	}

	int32 margin = 0;
	int32 spacing = 0;
	int32 tileWidth = 0;
	int32 tileHeight = 0;
	readInt(input, margin);
	readInt(input, spacing);
	readInt(input, tileWidth);
	readInt(input, tileHeight);

	ss << endl << margin << ' ' << spacing << ' ' << tileWidth << ' ' << tileHeight;

	input.close();
	return ss.str();
}

// TODO ensure correct endianness
void readFloat(ifstream& input, float32& value) {
	char b[4] = {0, 0, 0, 0};
	
	input.get(b[0]).get(b[1]).get(b[2]).get(b[3]);

	memcpy(&value, b, 4);
}

void readInt(ifstream& input, int32& value) {
	char b[4] = { 0, 0, 0, 0 };
	
	input.get(b[0]).get(b[1]).get(b[2]).get(b[3]);

	memcpy(&value, b, 4);
}