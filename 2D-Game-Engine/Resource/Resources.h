#pragma once

#include "Core/Core.h"

#include <utility>

class RawImage;
class TileScene;
class Game;
struct Glyph;
class Font;

string loadSrc(const string& file);
uint8* loadTexture(const string& file, uint8 type, uint32& width, uint32& height, uint8& channels);
uint8* loadTexture(const string& file, uint8 type, uint32& width, uint32& height, uint8& channels, uint32& depth);
RawImage* loadImage(string file);
RawImage* loadImages(string file, int32 margin, int32 spacing, int32 tileWidth, int32 tileHeight, uint32& imgCount);
RawImage* loadImages(const string& imgFile, const string& infoFile, uint32& numberOfImages);
std::pair<char, Glyph>* loadFont(const string& file, uint32& charMapSize, Glyph& invalidCharacter);
TileScene* loadTileLevel(string file);
