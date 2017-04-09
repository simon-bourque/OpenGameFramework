#ifndef SCENE_H
#define SCENE_H

#include "Rectangle.h"
#include "Core.h"

class Game;
class RenderSystem;

class Scene {
private:
	Rectangle m_bounds;
protected:
	void renderObjects(RenderSystem* rs);
	void renderBackgrounds(RenderSystem* rs);
public:
	Scene(const Rectangle& bounds);
	virtual ~Scene();

	virtual void tick(float32 delta, Game* game);
	virtual void render(RenderSystem* rs);

	const Rectangle& getBounds() const { return m_bounds; };
	Rectangle& getBounds() { return m_bounds; };
};

#endif