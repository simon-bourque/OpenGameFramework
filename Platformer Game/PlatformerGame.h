#ifndef PLATFORMERGAME_H
#define PLATFORMERGAME_H

#include "Game.h"

#include "Transform.h"

class Texture;

class PlatformerGame : public Game
{

	friend void onKeyPressed(void* object, int32 key, int32 scancode, int32 action, int32 mods);

private:
	Transform testT;
	Texture* testTexture;

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
};

#endif

