#include "Scene.h"



Scene::Scene(const Rectangle& bounds) : m_bounds(bounds) {}

Scene::~Scene() {}

void Scene::tick(float32 delta, Game* game) {
	// TODO update objects
}

void Scene::render(const RenderSystem& rs) {
	renderBackgrounds(rs);
	renderObjects(rs);
}

void Scene::renderObjects(const RenderSystem& rs) {
	// TODO render objects
}

void Scene::renderBackgrounds(const RenderSystem& rs) {
	// TODO render backgrounds
}
