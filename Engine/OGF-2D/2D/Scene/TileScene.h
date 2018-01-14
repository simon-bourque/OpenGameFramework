#pragma once

#include "Core/Core.h"

#include "2D/Scene/Scene2D.h"

#include <vector>

class TileLayer;

class TileScene : public Scene2D {
private:
	std::vector<TileLayer*> m_layers;
public:
	TileScene(const geo::Rectangle& bounds);
	virtual ~TileScene();

	void addTileLayer(TileLayer* layer);

	virtual void render() override;
};