#pragma once

#include "Core/Core.h"

#include <memory>

struct Tile;
class Texture;
class VertexArrayObject;

class TileLayer {
private:
	Texture* m_tileSheet;

	std::unique_ptr<VertexArrayObject> m_tileVAO;

	int32 m_numberOfTiles;
public:
	TileLayer(Tile* tiles, int32 numberOfTiles, Texture* tileSheet);
	virtual ~TileLayer();

	void render();
};

