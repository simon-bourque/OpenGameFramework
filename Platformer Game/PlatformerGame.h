#ifndef PLATFORMERGAME_H
#define PLATFORMERGAME_H

#include "Game.h"

#include "Transform.h"
#include "Texture.h"

class PlatformerGame : public Game
{
private:
	Transform testT;
	Texture* testTexture;
public:
	PlatformerGame();
	virtual ~PlatformerGame();

	virtual void init() override;
	virtual void render() override;
};

#endif

