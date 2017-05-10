#include "GameObject.h"

#include "Object/Component/ObjectComponent.h"
#include "Object/Component/RenderableComponent.h"

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

void GameObject::render(RenderSystem* rs) {
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

ObjectComponent* GameObject::findComponent(ComponentType type) {
	for (ObjectComponent* component : m_components) {
		if (component->getType() == type) {
			return component;
		}
	}
	for (RenderableComponent* component : m_renderableComponents) {
		if (component->getType() == type) {
			return component;
		}
	}

	return nullptr;
}