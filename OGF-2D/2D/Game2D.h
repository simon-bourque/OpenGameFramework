#pragma once

#include "Core/Core.h"
#include "Core/Game.h"

namespace geo {
	class Rectangle;
}

class Game2D : Game {
public:
	Game2D(const string& title, int32 width, int32 height, const geo::Rectangle& viewPort);
	virtual ~Game2D();

	virtual void tick(float32 deltaSeconds) override;
	virtual void render() override;
	virtual void init() = 0;
};

