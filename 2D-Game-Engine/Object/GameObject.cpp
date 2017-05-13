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

void GameObject::tick(float32 delta) {
	for (ObjectComponent* component : m_components) {
		if (component->isActive()) {
			component->tick(delta);
		}
	}
	for (RenderableComponent* component : m_renderableComponents) {
		if (component->isActive()) {
			component->tick(delta);
		}
	}
}

void GameObject::broadcastEvent(const Event& event) {
	for (ObjectComponent* component : m_components) {
		if (component->isActive()) {
			component->receiveEvent(event);
		}
	}
	for (RenderableComponent* component : m_renderableComponents) {
		if (component->isActive()) {
			component->receiveEvent(event);
		}
	}
}

void GameObject::render() {
	for (RenderableComponent* component : m_renderableComponents) {
		if (component->isVisible()) {
			component->render();
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

#ifdef DEBUG_BUILD
void GameObject::debugRender() {
	for (ObjectComponent* component : m_components) {
		component->debugRender();
	}
	for (RenderableComponent* component : m_renderableComponents) {
		component->debugRender();
	}
}
#endif