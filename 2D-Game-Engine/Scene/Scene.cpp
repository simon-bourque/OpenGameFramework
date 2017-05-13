#include "Scene.h"

#include "Object/GameObject.h"

#include "Physics/Collision/CollisionSystem.h"

#include "Graphics/Background.h"

Scene::Scene(const Rectangle& bounds) : m_bounds(bounds), m_gravity(0.0f) {
	m_collisionSystem.reset(new CollisionSystem(bounds));
}

Scene::~Scene() {
	for (GameObject* object : m_objects) {
		delete object;
	}
	for (Background* bg : m_backgrounds) {
		delete bg;
	}
}

void Scene::tick(float32 delta) {
	for (GameObject* object : m_objects) {
		object->tick(delta);
	}
}

void Scene::render() {
	renderBackgrounds();
	renderObjects();
}

void Scene::addGameObject(GameObject* object) {
	m_objects.push_back(object);
}

void Scene::addBackground(Background* background) {
	m_backgrounds.push_back(background);
}

void Scene::renderObjects() {
	for (GameObject* object : m_objects) {
		object->render();
#ifdef DEBUG_BUILD
		object->debugRender();
#endif
	}
}

void Scene::renderBackgrounds() {
	for (Background* bg : m_backgrounds) {
		bg->render();
	}
}
