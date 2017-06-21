#pragma once

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

	ObjectComponent* findComponent(ComponentType type);

#ifdef DEBUG_BUILD
	void debugRender();
#endif
};