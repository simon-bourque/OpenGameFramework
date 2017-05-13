#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Core/Core.h"

#include "Object/Transform.h"

#include "Object/Component/ComponentType.h"

#include <vector>

class ObjectComponent;
class RenderableComponent;
class Game;
class RenderSystem;
struct Event;

class GameObject
{
private:
	Transform m_transform;

	std::vector<ObjectComponent*> m_components;
	std::vector<RenderableComponent*> m_renderableComponents;
public:
	GameObject();
	explicit GameObject(const Transform& transform);
	virtual ~GameObject();

	void tick(float32 delta);
	void render();

	void addComponent(ObjectComponent* component);
	void addComponent(RenderableComponent* component);

	void broadcastEvent(const Event& event);

	ObjectComponent* findComponent(ComponentType type);

	const Transform& getTransform() const { return m_transform; };
	Transform& getTransform() { return m_transform; };
};

#endif