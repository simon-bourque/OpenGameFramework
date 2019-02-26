#pragma once

#include "Core/Math/Vector2f.h"

#include "2D/Object/GameObject.h"

struct Manifold;

class SpriteComponent;
class AABBColliderComponent;
class EmitterComponent;

class Ball : public GameObject
{
public:
	Ball();
	virtual ~Ball();
	Ball(const Ball& other) = delete;
	Ball(Ball&& other) = delete;
	Ball& operator=(const Ball& other) = delete;
	Ball& operator=(Ball&& other) = delete;

	virtual void tick(float32 delta) override;
	void onCollision(const Manifold& manifold, GameObject* collidedWith);
	void pushBack(const Manifold& manifold); // Push back the ball out of the collider

	bool isDocked() const { return m_docked; }
	bool isOutOfBounds() const { return m_outOfBounds; }

	void setDocked(bool docked); // Releases or docks the ball

	void activateXBallMode(); // Activate Xball mode (no collisions with bricks)
	void activateNormalMode(); // Activate initial mode

private:
	// Ball Constants
	constexpr static float32 UNDOCKING_VEL = 30.0f;
	constexpr static float32 BALL_WIDTH = 1;
	constexpr static float32 BALL_HEIGHT = 1;
	constexpr static float32 BALL_SCALE = 1.0f;
	constexpr static float32 FORCE_CONSTANT = 0.10f; // Proportional force to apply depending on shimmy rate of paddle
	constexpr static float32 XBALL_TIMER = 5.0f;
	constexpr static float32 MAX_VEL = 75.0f;
	
	SpriteComponent* m_sprite = nullptr;
	AABBColliderComponent* m_collider = nullptr;
	EmitterComponent* m_emitter = nullptr;

	// Ball state
	Vector2f m_vel;
	float32 m_timer;
	bool m_docked;
	bool m_outOfBounds;
	bool m_xBallMode;
	bool m_hasFlipped; // If ball has already collided during the frame
};