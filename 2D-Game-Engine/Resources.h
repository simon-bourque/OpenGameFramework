#ifndef RESOURCES_H
#define RESOURCES_H

#include "Core.h"

class RawImage;
class TileScene;
class Game;
class Font;

string loadSrc(string file);
RawImage* loadImage(string file);
RawImage* loadImages(string file, int32 margin, int32 spacing, int32 tileWidth, int32 tileHeight, int32& imgCount);
Font* loadFont(string fontAtlas, string fontDesc, Game* game);
TileScene* loadTileLevel(string file, Game* game);

#endif
