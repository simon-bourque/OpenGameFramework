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
std::pair<char, Glyph>* loadFont(const string& file, uint32& charMapSize, Glyph& invalidCharacter);
TileScene* loadTileLevel(string file);
