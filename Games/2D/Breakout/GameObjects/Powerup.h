#pragma once

#include "Core/Math/Vector2f.h"

#include "2D/Object/GameObject.h"
#include "2D/Physics/Collision/Manifold.h"

class SpriteComponent;
class AABBColliderComponent;

class Powerup : public GameObject
{
public:

	enum class PowerType {
		XBALL = 0,
		REBUILD = 1,
	};

	enum class PowerState {
		IDLE,
		FALLING,
		ACTIVE
	};

	Powerup();
	virtual ~Powerup();
	Powerup(const Powerup& other) = delete;
	Powerup(Powerup&& other) = delete;
	Powerup& operator=(const Powerup& other) = delete;
	Powerup& operator=(Powerup&& other) = delete;

	virtual void tick(float32 delta) override;
	virtual void render() override;

	void onCollision(const Manifold& manifold, GameObject* collidedWith);

	bool consume();
	void reset();
	PowerType getType() { return m_type; }

	void spawnRandomType(Vector2f position);

	constexpr static float32 POWERUP_WIDTH = 1;
	constexpr static float32 POWERUP_HEIGHT = 1;

private:

	Vector2f m_vel;
	PowerType m_type;
	PowerState m_state;

	SpriteComponent* m_sprite = nullptr;
	AABBColliderComponent* m_collider = nullptr;

};

