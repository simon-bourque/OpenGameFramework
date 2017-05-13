#pragma once
#ifndef GAME_H
#define GAME_H

#include "Core/Core.h"

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
	bool m_shutdown;
	int32 m_fps;
public:
	Game(const string& title, int32 width, int32 height, const Rectangle& viewPort);
	virtual ~Game();

	void run();

	virtual void tick(float32 delta);
	virtual void render();
	virtual void init() = 0;

	void shutdown() { m_shutdown = true; };

	int32 getFps() const { return m_fps; };
};

#endif

