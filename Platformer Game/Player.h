#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "Object\GameObject.h"

class Game;
struct Vector2f;

class Player : public GameObject {
public:
	Player(Game* game, const Vector2f& spawnLocation);
	virtual ~Player();
};

#endif