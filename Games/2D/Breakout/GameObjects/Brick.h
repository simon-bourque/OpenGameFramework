#pragma once

#include "Core/Math/Vector2f.h"

#include "2D/Object/GameObject.h"
#include "2D/Physics/Collision/Manifold.h"

#include <mutex>

class Level;
class SpriteComponent;
class AABBColliderComponent;

class Brick : public GameObject
{
public:

	enum class BrickType {
		NORMAL
	};

	enum class BrickState {
		VISIBLE,
		DESTROYED
	};

	Brick();
	Brick(Level* level, BrickType type, Vector2f position);
	virtual ~Brick();
	Brick(const Brick& other) = delete;
	Brick(Brick&& other) = delete;
	Brick& operator=(const Brick& other) = delete;
	Brick& operator=(Brick&& other) = delete;

	BrickType getType() const { return m_type; }
	BrickState getState() const { return m_state; }
	Vector2f getPosition() const { return m_position; }
	bool isVisible() const { return m_state == BrickState::VISIBLE; }
	void reset();

	void onCollision(const Manifold& manifold, GameObject* collidedWith);

	constexpr static float32 BRICK_WIDTH = 4;
	constexpr static float32 BRICK_HEIGHT = 1;

private:

	constexpr static float32 POWERUP_CHANCE = 10; // In %

	std::mutex m_mutex;

	Level* m_level = nullptr;
	SpriteComponent* m_sprite = nullptr;
	AABBColliderComponent* m_collider = nullptr;

	Vector2f m_position;
	BrickType m_type;
	BrickState m_state;

};