#ifndef GAME_H
#define GAME_H

#include "Core.h"

class Window;
class RenderSystem;
class Rectangle;

class Game
{
private:
	Window* m_window;
	RenderSystem* m_renderSystem;

	bool m_shutdown;
	int32 m_fps;
public:
	Game(string title, int32 width, int32 height, const Rectangle& viewPort);
	virtual ~Game();

	void run();

	virtual void tick(float32 delta);
	virtual void render();
	virtual void init() = 0;

	void shutdown() { m_shutdown = true; };

	Window* getWindow() const { return m_window; };
	RenderSystem* getRenderSystem() const { return m_renderSystem; };
};

#endif

