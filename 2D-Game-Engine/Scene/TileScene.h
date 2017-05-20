#pragma once
#ifndef TILESCENE_H
#define TILESCENE_H

#include "Core/Core.h"

#include "Scene/Scene.h"

#include <vector>

class TileLayer;

class TileScene : public Scene {
private:
	std::vector<TileLayer*> m_layers;
public:
	TileScene(TileLayer* layer, const Rectangle& bounds);
	virtual ~TileScene();

	void addTileLayer(TileLayer* layer);

	virtual void render() override;
};

#endif