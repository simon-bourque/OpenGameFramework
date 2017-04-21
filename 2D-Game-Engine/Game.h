#ifndef GAME_H
#define GAME_H

#include "Core.h"

#include <memory>

class Window;
class RenderSystem;
class Rectangle;

class Game
{
private:
	unique_ptr<Window> m_window;
	unique_ptr<RenderSystem> m_renderSystem;

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

	Window* getWindow() const { return m_window.get(); };
	RenderSystem* getRenderSystem() const { return m_renderSystem.get(); };
};

#endif

