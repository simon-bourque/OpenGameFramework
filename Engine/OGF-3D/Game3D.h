#pragma once

#include "Core/Core.h"
#include "Core/Game.h"

class Game3D : public Game {
public:
	Game3D(const string& title, int32 width, int32 height);
	virtual ~Game3D();

	virtual void tick(float32 deltaSeconds) override;
	virtual void render() override;
	virtual void init() = 0;
};

