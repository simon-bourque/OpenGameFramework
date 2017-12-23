#pragma once

#include "Core/Core.h"

#include "Core/Math/Geometry/Rectangle.h"

#include <vector>
#include <memory>

class Game;
class RenderSystem;
class GameObject;
class CollisionSystem;
class Background;

class Scene {
private:
	geo::Rectangle m_bounds;

	std::vector<GameObject*> m_objects;
	std::vector<Background*> m_backgrounds;

	std::unique_ptr<CollisionSystem> m_collisionSystem;

	float32 m_gravity;
protected:
	void renderObjects();
	void renderBackgrounds();
public:
	Scene(const geo::Rectangle& bounds);
	virtual ~Scene();

	virtual void tick(float32 delta);
	virtual void render();

	void addGameObject(GameObject* object);
	void addBackground(Background* background);

	const geo::Rectangle& getBounds() const { return m_bounds; };
	geo::Rectangle& getBounds() { return m_bounds; };

	float32 getGravity() const { return m_gravity; };
	void setGravity(float32 gravity) { m_gravity = gravity; };

	CollisionSystem* getCollisionSystem() { return m_collisionSystem.get(); };
};