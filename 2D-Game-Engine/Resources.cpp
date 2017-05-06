
#include "Core.h"

#include "RawImage.h"
#include "TileScene.h"
#include "Tile.h"
#include "Game.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Font.h"
#include "Glyph.h"
#include "Vector2f.h"

#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <sstream>
#include <fstream>

#include <cstdlib>
#include <map>

void readFloat(std::ifstream& input, float32& value);
void readInt(std::ifstream& input, int32& value);

const static string SHADER_PATH = "res/shader/";
const static string TEXTURE_PATH = "res/texture/";
const static string LEVEL_PATH = "res/level/";
const static string FONT_PATH = "res/font/";

string loadSrc(string file) {
	std::ifstream input(SHADER_PATH + file);

	if (!input) {
		input.close();
		throw std::runtime_error("Failed to load source from file " + file);
	}

	std::stringstream ss;

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

RawImage* loadImage(string file) {
	int32 width = 0;
	int32 height = 0;
	int32 channels = 0;
	uint8* data = stbi_load((TEXTURE_PATH + file).c_str(), &width, &height, &channels, 4);

	if (!data) {
		const char* reason = stbi_failure_reason();
		throw std::runtime_error("Failed to load texture " + file + ": " + reason);
	}

	RawImage* img = new RawImage(data, width, height, channels);

	stbi_image_free(data);

	return img;
}

RawImage* loadImages(string file, int32 margin, int32 spacing, int32 tileWidth, int32 tileHeight, uint32& imgCount) {
	RawImage* img = loadImage(file);

	int32 numTilesPerWidth = (img->getWidth() - (margin * 2) + spacing) / (tileWidth + spacing);
	int32 numTilesPerHeight = (img->getHeight() - (margin * 2) + spacing) / (tileHeight + spacing);

	int32 maxWidth = (numTilesPerWidth * tileWidth) + (spacing * (numTilesPerWidth - 1)) + margin;
	int32 maxHeight = (numTilesPerHeight * tileHeight) + (spacing * (numTilesPerHeight - 1)) + margin;

	RawImage* imgs = new RawImage[numTilesPerWidth * numTilesPerHeight];

	int32 tileCount = 0;
	for (int32 y = margin; y < maxHeight; y += tileHeight + spacing) {
		for (int32 x = margin; x < maxWidth; x += tileWidth + spacing) {

			RawImage subImage = img->getSubImage(x, y, tileWidth, tileHeight);

			imgs[tileCount++] = subImage;
		}
	}

	delete img;

	imgCount = numTilesPerWidth * numTilesPerHeight;
	return imgs;
}

std::pair<char, Glyph>* loadFont(const string& file, uint32& charMapSize, Glyph& invalidCharacter) {

	std::ifstream input(FONT_PATH + file);
	
	if (!input) {
		input.close();
		throw std::runtime_error("Failed to load font " + file);
	}

	string read;
	// Ignore first line
	getline(input, read);

	// Extract atlas width and height
	input >> read;
	input >> read;
	input >> read;

	input >> read;
	float32 imgWidth = stoi(read.substr(7));
	input >> read;
	float32 imgHeight = stoi(read.substr(7));
	getline(input, read);

	// Ignore next line
	getline(input, read);

	// Extract number of characters
	getline(input, read);
	uint32 numCharacters = stoi(read.substr(12));

	// TODO handle not having a invalid character
	charMapSize = numCharacters - 1;
	std::pair<char, Glyph>* characterMap = new std::pair<char, Glyph>[charMapSize];
	
	uint32 charMapCounter = 0;
	for (uint32 i = 0; i < numCharacters; i++) {
		
		input >> read;
		input >> read;
		int32 asciiCode = stoi(read.substr(3));

		input >> read;
		float32 x = stoi(read.substr(2)) / imgWidth;
		input >> read;
		float32 y = stoi(read.substr(2)) / imgHeight;
		input >> read;
		float32 width = stoi(read.substr(6)) / imgWidth;
		input >> read;
		float32 height = stoi(read.substr(7)) / imgHeight;
		input >> read;
		float32 xOffset = stoi(read.substr(8)) / imgWidth;
		input >> read;
		float32 yOffset = stoi(read.substr(8)) / imgHeight;
		input >> read;
		float32 xAdvance = stoi(read.substr(9)) / imgWidth;

		input >> read;
		input >> read;

		// Compute text coords and vertices
		float32 textCoords[12];
		float32 vertices[12];

		Vector2f bottomLeft(x, y + height);
		Vector2f topLeft(x, y);
		Vector2f bottomRight(x + width, y + height);
		Vector2f topRight(x + width, y);

		textCoords[0] = bottomLeft.x;
		textCoords[1] = bottomLeft.y;
		textCoords[2] = topLeft.x;
		textCoords[3] = topLeft.y;
		textCoords[4] = bottomRight.x;
		textCoords[5] = bottomRight.y;
		textCoords[6] = bottomRight.x;
		textCoords[7] = bottomRight.y;
		textCoords[8] = topLeft.x;
		textCoords[9] = topLeft.y;
		textCoords[10] = topRight.x;
		textCoords[11] = topRight.y;

		vertices[0] = 0; // bottom left x
		vertices[1] = -height; // bottom left y
		vertices[2] = 0; // top left x
		vertices[3] = 0; // top left y
		vertices[4] = width; // bottom right x
		vertices[5] = -height; // bottom right y
		vertices[6] = width; // bottom right x
		vertices[7] = -height; // bottom right y
		vertices[8] = 0; // top left x
		vertices[9] = 0; // top left y
		vertices[10] = width; // top right x
		vertices[11] = 0; // top right y

		if (asciiCode == -1) {
			invalidCharacter = Glyph(i, width, height, xOffset, yOffset, xAdvance, textCoords, vertices);
		}
		else {
			characterMap[charMapCounter++] = std::pair<char, Glyph>(asciiCode, Glyph(i, width, height, xOffset, yOffset, xAdvance, textCoords, vertices));
		}
	}

	input.close();
	return characterMap;
}

TileScene* loadTileLevel(string file, Game* game) {
	std::ifstream input(LEVEL_PATH + file);

	if (!input) {
		input.close();
		throw std::runtime_error("Failed to load level " + file);
	}


	// Load level bounds
	float32 boundsX = 0;
	float32 boundsY = 0;
	int32 boundsWidth = 0;
	int32 boundsHeight = 0;
	readFloat(input, boundsX);
	readFloat(input, boundsY);
	readInt(input, boundsWidth);
	readInt(input, boundsHeight);

	int32 numTiles = 0;
	readInt(input, numTiles);

	
	Tile* tiles = new Tile[numTiles];
	for (int32 i = 0; i < numTiles; i++) {
		float32 x = 0;
		float32 y = 0;
		int32 index = 0;
		readFloat(input, x);
		readFloat(input, y);
		readInt(input, index);
		
		tiles[i] = { x, y, index };
	}

	int32 numColliders = 0;
	readInt(input, numColliders);

	for (int32 i = 0; i < numColliders; i++) {
		float32 x = 0;
		float32 y = 0;
		float32 width = 0;
		float32 height = 0;
		readFloat(input, x);
		readFloat(input, y);
		readFloat(input, width);
		readFloat(input, height);
	}

	int32 stringLen = 0;
	readInt(input, stringLen);

	char* texPath = new char[stringLen + 1];

	for (int32 i = 0; i < stringLen; i++) {
		char character = 0;
		input.get(character);
		texPath[i] = character;
	}
	texPath[stringLen] = '\0';

	DEBUG_LOG("--- TILE SHEET PATH ---");
	DEBUG_LOG(texPath);

	int32 margin = 0;
	int32 spacing = 0;
	int32 tileWidth = 0;
	int32 tileHeight = 0;
	readInt(input, margin);
	readInt(input, spacing);
	readInt(input, tileWidth);
	readInt(input, tileHeight);

	Texture* texture = game->getRenderSystem().getTextureManager()->createTexture2DArray(texPath, margin, spacing, tileWidth, tileHeight, Texture::Filter::NEAREST_NEIGHBOR);

	input.close();

	TileScene* scene = new TileScene(tiles, numTiles, texture, Rectangle(boundsX, boundsY, boundsWidth, boundsHeight));

	delete[] tiles;
	delete[] texPath;

	return scene;
}

// TODO ensure correct endianness
void readFloat(std::ifstream& input, float32& value) {
	char b[4] = {0, 0, 0, 0};
	
	input.get(b[0]).get(b[1]).get(b[2]).get(b[3]);

	memcpy(&value, b, 4);
}

void readInt(std::ifstream& input, int32& value) {
	char b[4] = { 0, 0, 0, 0 };
	
	input.get(b[0]).get(b[1]).get(b[2]).get(b[3]);

	memcpy(&value, b, 4);
}