#include "GameObject.h"

#include "ObjectComponent.h"
#include "RenderableComponent.h"

GameObject::GameObject() {}

GameObject::GameObject(const Transform& transform) : m_transform(transform) {}

GameObject::~GameObject() {
	for (ObjectComponent* component : m_components) {
		delete component;
	}
	for (RenderableComponent* component : m_renderableComponents) {
		delete component;
	}
}

void GameObject::tick(float32 delta, Game* game) {
	for (ObjectComponent* component : m_components) {
		if (component->isActive()) {
			component->tick(delta, game);
		}
	}
	for (RenderableComponent* component : m_renderableComponents) {
		if (component->isActive()) {
			component->tick(delta, game);
		}
	}
}

void GameObject::render(const RenderSystem& rs) {
	for (RenderableComponent* component : m_renderableComponents) {
		if (component->isVisible()) {
			component->render(rs);
		}
	}
}

void GameObject::addComponent(ObjectComponent* component) {
	m_components.push_back(component);
}

void GameObject::addComponent(RenderableComponent* component) {
	m_renderableComponents.push_back(component);
}