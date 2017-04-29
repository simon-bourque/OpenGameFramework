#ifndef GAME_H
#define GAME_H

#include "Core.h"

#include <memory>

class Window;
class RenderSystem;
class Rectangle;
class SceneManager;

#ifdef DEBUG_BUILD
class Debug;
#endif

class Game
{
private:
	unique_ptr<Window> m_window;
	unique_ptr<RenderSystem> m_renderSystem;
	unique_ptr<SceneManager> m_sceneManager;

#ifdef DEBUG_BUILD
	unique_ptr<Debug> m_debug;
#endif

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

	Window& getWindow() { return *m_window; };
	RenderSystem& getRenderSystem() { return *m_renderSystem; };
	SceneManager& getSceneManager() { return *m_sceneManager; };
};

#endif

