#pragma once

#include "Core/Core.h"

#include "Object/Transform.h"

#include "Object/Component/ComponentType.h"
#include "Object/Component/ObjectComponent.h"
#include "Object/Component/RenderableComponent.h"

#include <vector>

class Game;
class RenderSystem;
struct Event;

class GameObject
{
public:
	Transform transform;
private:
	std::vector<ObjectComponent*> m_components;
	std::vector<RenderableComponent*> m_renderableComponents;
public:
	GameObject();
	explicit GameObject(const Transform& transform);
	virtual ~GameObject();

	virtual void tick(float32 delta);
	void render();

	void addComponent(ObjectComponent* component);
	void addComponent(RenderableComponent* component);

	void broadcastEvent(const Event& event);

	template<typename T>
	T* findComponent() const;

#ifdef DEBUG_BUILD
	void debugRender();
#endif
};

template<typename T>
T* GameObject::findComponent() const {
	static_assert(std::is_base_of<ObjectComponent, T>::value, "Type must derive from ObjectComponent.");
	int64 id = ComponentType<T>::id();

	if (id == -1) {
		return nullptr;
	}

	for (ObjectComponent* component : m_components) {
		if (component->getTypeId() == id) {
			return static_cast<T*>(component);
		}
	}
	for (ObjectComponent* component : m_renderableComponents) {
		if (component->getTypeId() == id) {
			return static_cast<T*>(component);
		}
	}

	return nullptr;
}
