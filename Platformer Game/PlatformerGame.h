#ifndef PLATFORMERGAME_H
#define PLATFORMERGAME_H

#include "Game.h"

#include "Transform.h"

class Texture;
class TileScene;

class PlatformerGame : public Game
{
private:
	TileScene* scene;

	bool up;
	bool down;
	bool left;
	bool right;
public:
	PlatformerGame();
	virtual ~PlatformerGame();

	virtual void init() override;
	virtual void render() override;
	virtual void tick(float32 delta) override;

	void onKeyPress(int32 key, int32 scancode, int32 action, int32 mods);
	void onMouseMove(float64 xPos, float64 yPos);
};

#endif

