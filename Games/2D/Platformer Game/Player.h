#pragma once

#include "2D/Object/GameObject.h"

class Game2D;
struct Vector2f;

class Player : public GameObject {
public:
	Player(Game2D* game, const Vector2f& spawnLocation);
	virtual ~Player();
};