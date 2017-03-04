#ifndef GAME_H
#define GAME_H

#include "Core.h"

class Window;

class Game
{
private:
	Window* m_window;

	bool m_shutdown;
	int32 m_fps;
public:
	Game(string title, int32 width, int32 height);
	virtual ~Game();

	void run();

	virtual void tick(float32 delta);
	virtual void render();
	virtual void init() = 0;

	void shutdown() { m_shutdown = true; };
};

#endif

