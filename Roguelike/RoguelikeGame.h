#pragma once

#include "Core/Game.h"

class RoguelikeGame : public Game {
public:
	RoguelikeGame();
	virtual ~RoguelikeGame();

	virtual void init() override;
	virtual void render() override;
	virtual void tick(float32 delta) override;
};

