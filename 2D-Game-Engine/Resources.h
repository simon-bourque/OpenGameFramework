#ifndef RESOURCES_H
#define RESOURCES_H

#include "Core.h"
#include <utility>

class RawImage;
class TileScene;
class Game;
class Glyph;
class Font;

string loadSrc(string file);
RawImage* loadImage(string file);
RawImage* loadImages(string file, int32 margin, int32 spacing, int32 tileWidth, int32 tileHeight, int32& imgCount);
std::pair<char, Glyph>* loadFont(const string& file, uint32& charMapSize, Glyph& invalidCharacter);
TileScene* loadTileLevel(string file, Game* game);

#endif
