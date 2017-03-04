#ifndef PLATFORMERGAME_H
#define PLATFORMERGAME_H

#include "Game.h"

class PlatformerGame : public Game
{
public:
	PlatformerGame();
	virtual ~PlatformerGame();

	virtual void init() override;
};

#endif

