#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Transform.h"

#include "Core.h"

#include <vector>

class ObjectComponent;
class RenderableComponent;
class Game;
class RenderSystem;

class GameObject
{
private:
	Transform m_transform;

	std::vector<ObjectComponent*> m_components;
	std::vector<RenderableComponent*> m_renderableComponents;
public:
	GameObject();
	explicit GameObject(const Transform& transform);
	~GameObject();

	void tick(float32 delta, Game* game);
	void render(RenderSystem* rs);

	void addComponent(ObjectComponent* component);
	void addComponent(RenderableComponent* component);

	const Transform& getTransform() const { return m_transform; };
	Transform& getTransform() { return m_transform; };
};

#endif