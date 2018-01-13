#pragma once

#include "Core/Core.h"

#include "Core/Graphics/TextureCache.h"

#include <memory>

struct Tile;
class VertexArrayObject;

class TileLayer {
private:
	TextureRef m_tileSheet;

	std::unique_ptr<VertexArrayObject> m_tileVAO;

	int32 m_numberOfTiles;
public:
	TileLayer(Tile* tiles, int32 numberOfTiles, TextureRef tileSheet);
	virtual ~TileLayer();

	void render();
};

