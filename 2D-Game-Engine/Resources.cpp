
#include "Core.h"

#include "RawImage.h"
#include "TileScene.h"
#include "Tile.h"
#include "Game.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <sstream>
#include <fstream>

#include <cstdlib>

void readFloat(ifstream& input, float32& value);
void readInt(ifstream& input, int32& value);

const static string SHADER_PATH = "res/shader/";
const static string TEXTURE_PATH = "res/texture/";
const static string LEVEL_PATH = "res/level/";

string loadSrc(string file) {
	ifstream input(SHADER_PATH + file);

	if (!input) {
		input.close();
		throw runtime_error("Failed to load source from file " + file);
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

RawImage* loadImage(string file) {
	int32 width = 0;
	int32 height = 0;
	int32 channels = 0;
	uint8* data = stbi_load((TEXTURE_PATH + file).c_str(), &width, &height, &channels, 4);
	
	RawImage* img = new RawImage(data, width, height, channels);

	free(data); // stb uses malloc

	return img;
}

RawImage* loadImages(string file, int32 margin, int32 spacing, int32 tileWidth, int32 tileHeight, int32& imgCount) {
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

TileScene* loadTileLevel(string file, Game* game) {
	ifstream input(LEVEL_PATH + file);

	if (!input) {
		input.close();
		throw runtime_error("Failed to load level " + file);
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

	int32 imgCount = 0;
	//RawImage* imgs = loadImages(texPath, margin, spacing, tileWidth, tileHeight, imgCount);
	RawImage* imgs = loadImages(texPath, margin, spacing, tileWidth, tileHeight, imgCount);

	Texture* texture = game->getRenderSystem().getTextureManager()->createTexture2DArray(imgs, imgCount, Texture::Filter::NEAREST_NEIGHBOR);

	delete[] imgs;

	input.close();

	TileScene* scene = new TileScene(tiles, numTiles, texture, Rectangle(boundsX, boundsY, boundsWidth, boundsHeight));

	delete[] tiles;
	delete[] texPath;

	return scene;
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