#pragma once

#include "Core/Core.h"

#include "2D/Scene/Scene.h"

#include <vector>

class TileLayer;

class TileScene : public Scene {
private:
	std::vector<TileLayer*> m_layers;
public:
	TileScene(const geo::Rectangle& bounds);
	virtual ~TileScene();

	void addTileLayer(TileLayer* layer);

	virtual void render() override;
};