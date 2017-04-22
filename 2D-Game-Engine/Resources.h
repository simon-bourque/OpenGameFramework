#ifndef RESOURCES_H
#define RESOURCES_H

#include "Core.h"

class RawImage;
class TileScene;
class Game;

string loadSrc(string file);
RawImage* loadImage(string file);
RawImage* loadImages(string file, int32 margin, int32 spacing, int32 tileWidth, int32 tileHeight, int32& imgCount);
TileScene* loadTileLevel(string file, Game* game);

#endif
