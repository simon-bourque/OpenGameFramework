#include "Scene2D.h"

#include "2D/Object/GameObject.h"

#include "2D/Physics/Collision/CollisionSystem.h"

#include "2D/Scene/Background.h"

Scene2D::Scene2D(const geo::Rectangle& bounds) : m_bounds(bounds), m_gravity(0.0f) {
	m_collisionSystem.reset(new CollisionSystem(bounds));
}

Scene2D::~Scene2D() {
	for (GameObject* object : m_objects) {
		delete object;
	}
	for (Background* bg : m_backgrounds) {
		delete bg;
	}
}

void Scene2D::tick(float32 delta) {
	for (GameObject* object : m_objects) {
		object->tick(delta);
	}
}

void Scene2D::render() {
	renderBackgrounds();
	renderObjects();
}

void Scene2D::addGameObject(GameObject* object) {
	m_objects.push_back(object);
}

void Scene2D::addBackground(Background* background) {
	m_backgrounds.push_back(background);
}

void Scene2D::renderObjects() {
	for (GameObject* object : m_objects) {
		object->render();
#ifdef DEBUG_BUILD
		object->debugRender();
#endif
	}
}

void Scene2D::renderBackgrounds() {
	for (Background* bg : m_backgrounds) {
		bg->render();
	}
}
