#pragma once
#ifndef PLATFORMERGAME_H
#define PLATFORMERGAME_H

#include "Core/Game.h"

class Texture;
class TileScene;
class AnimatorComponent;

class PlatformerGame : public Game
{
private:
	bool up;
	bool down;
	bool left;
	bool right;

	AnimatorComponent* animComp;
public:
	PlatformerGame();
	virtual ~PlatformerGame();

	virtual void init() override;
	virtual void render() override;
	virtual void tick(float32 delta) override;

	void onKeyPress(int32 key, int32 scancode, int32 action, int32 mods);
};

#endif

