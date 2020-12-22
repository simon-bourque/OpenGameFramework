#pragma once

#include "2D/Object/GameObject.h"

class PaddleController;

class Paddle : public GameObject
{
public:
	Paddle();
	virtual ~Paddle();
	Paddle(const Paddle& other) = delete;
	Paddle(Paddle&& other) = delete;
	Paddle& operator=(const Paddle& other) = delete;
	Paddle& operator=(Paddle&& other) = delete;

	Vector2f getPosition() const;
	const PaddleController* getController() const { return m_controller; }

	// Paddle Constants
	constexpr static float32 PADDLE_WIDTH = 5;
	constexpr static float32 PADDLE_HEIGHT = 1;
	constexpr static float32 PADDLE_YOFFSET = -14.5 + PADDLE_HEIGHT/2; // Y-offset and paddle height
private:

	PaddleController* m_controller = nullptr;

};