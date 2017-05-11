#include "Scene.h"

#include "Object/GameObject.h"

#include "Physics/Collision/CollisionSystem.h"

Scene::Scene(const Rectangle& bounds) : m_bounds(bounds) {
	m_collisionSystem.reset(new CollisionSystem(bounds));
}

Scene::~Scene() {
	for (GameObject* object : m_objects) {
		delete object;
	}
}

void Scene::tick(float32 delta, Game* game) {
	for (GameObject* object : m_objects) {
		object->tick(delta, game);
	}
}

void Scene::render(RenderSystem* rs) {
	renderBackgrounds(rs);
	renderObjects(rs);
}

void Scene::addGameObject(GameObject* object) {
	m_objects.push_back(object);
}

void Scene::renderObjects(RenderSystem* rs) {
	for (GameObject* object : m_objects) {
		object->render(rs);
	}
}

void Scene::renderBackgrounds(RenderSystem* rs) {
	// TODO render backgrounds
}
