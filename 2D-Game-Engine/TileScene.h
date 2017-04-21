#ifndef TILESCENE_H
#define TILESCENE_H

#include "Scene.h"
#include "Core.h"

struct Tile;
class Texture;
class VertexArrayObject;
class RenderSystem;

class TileScene : public Scene {
private:
	Texture* m_tileSheet;
	VertexArrayObject* m_tileVAO;

	int32 m_numberOfTiles;
public:
	TileScene(Tile* tiles, int32 numberOfTiles, Texture* tileSheet, const Rectangle& bounds);
	virtual ~TileScene();

	virtual void render(const RenderSystem& rs) override;
};

#endif