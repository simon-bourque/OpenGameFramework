#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Core/Core.h"

#include "Math/Geometry/Rectangle.h"

#include <vector>
#include <memory>

class Game;
class RenderSystem;
class GameObject;
class CollisionSystem;

class Scene {
private:
	Rectangle m_bounds;

	std::vector<GameObject*> m_objects;

	std::unique_ptr<CollisionSystem> m_collisionSystem;
protected:
	void renderObjects(RenderSystem* rs);
	void renderBackgrounds(RenderSystem* rs);
public:
	Scene(const Rectangle& bounds);
	virtual ~Scene();

	virtual void tick(float32 delta, Game* game);
	virtual void render(RenderSystem* rs);

	void addGameObject(GameObject* object);

	const Rectangle& getBounds() const { return m_bounds; };
	Rectangle& getBounds() { return m_bounds; };

	CollisionSystem* getCollisionSystem() { return m_collisionSystem.get(); };
};

#endif