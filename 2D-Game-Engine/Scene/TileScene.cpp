#include "TileScene.h"

#include "Scene/TileLayer.h"


TileScene::TileScene(TileLayer* layer, const Rectangle& bounds) : Scene(bounds) {
	m_layers.push_back(layer);
}


TileScene::~TileScene() {

}

void TileScene::addTileLayer(TileLayer* layer) {
	m_layers.push_back(layer);
}

void TileScene::render() {
	renderBackgrounds();

	for (TileLayer* layer : m_layers) {
		layer->render();
	}

	renderObjects();
}
