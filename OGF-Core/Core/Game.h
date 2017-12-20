#pragma once

#include "Core/Core.h"

class Game
{
private:
	bool m_shutdown;
	int32 m_fps;

	void startLoop();

protected:
	Game();
public:
	virtual ~Game();

	void start();

	virtual void tick(float32 delta) = 0;
	virtual void render() = 0;
	virtual void init() = 0;

	void shutdown() { m_shutdown = true; };

	int32 getFps() const { return m_fps; };
};
