#ifndef SCENE_H
#define SCENE_H

#include "Rectangle.h"
#include "Core.h"

#include <vector>

class Game;
class RenderSystem;
class GameObject;

class Scene {
private:
	Rectangle m_bounds;

	vector<GameObject*> m_objects;
protected:
	void renderObjects(const RenderSystem& rs);
	void renderBackgrounds(const RenderSystem& rs);
public:
	Scene(const Rectangle& bounds);
	virtual ~Scene();

	virtual void tick(float32 delta, Game* game);
	virtual void render(const RenderSystem& rs);

	void addGameObject(GameObject* object);

	const Rectangle& getBounds() const { return m_bounds; };
	Rectangle& getBounds() { return m_bounds; };
};

#endif