#include "Powerup.h"
#include "Paddle.h"

#include "Core/Graphics/GraphicsContext.h"
#include "Core/Graphics/TextureCache.h"
#include "Core/Assert.h"

#include "2D/Object/Component/SpriteComponent.h"
#include "2D/Object/Component/AABBColliderComponent.h"
#include "2D/Graphics/Graphics2D.h"

#include <stdlib.h>

Powerup::Powerup()
	: m_vel({ 0.0f, 0.0f })
	, m_type(PowerType::REBUILD)
	, m_state(PowerState::IDLE)
{
	transform.xScale = POWERUP_WIDTH;
	transform.yScale = POWERUP_HEIGHT;

	TextureRef puTextureRef = getGraphicsContextInstance()->getTextureCache()->loadTexture("rebuild_pu.tx");

	m_sprite = new SpriteComponent(this, puTextureRef);
	addComponent(m_sprite);

	m_collider = new AABBColliderComponent(this, geo::Rectangle(POWERUP_WIDTH, POWERUP_HEIGHT));
	m_collider->setActive(false);
	m_collider->setCollisionResponse(Delegate<const Manifold&, GameObject*>::create<Powerup, &Powerup::onCollision>(this));
	addComponent(m_collider);
}

void Powerup::spawnRandomType(Vector2f position)
{
	if (m_state == PowerState::IDLE)
	{
		// Powerup is now falling, active collisions
		m_collider->setActive(true);
		m_state = PowerState::FALLING;

		// Placing powerup at the origin of the brick
		transform.xPos = position.x;
		transform.yPos = position.y;

		// Choosing random powerup type
		int32 random = rand() % 2;

		switch (random)
		{
			case 0:
			{
				m_type = PowerType::XBALL;
				m_sprite->replaceTexture("indestructoball_pu.tx");
				m_vel.y = -5.0f;
				break;
			}
			case 1:
			{
				m_type = PowerType::REBUILD;
				m_sprite->replaceTexture("rebuild_pu.tx");
				m_vel.y = -30.0f;
				break;
			}
		}
	}
}

void Powerup::tick(float32 delta)
{
	GameObject::tick(delta);

	OGF_ASSERT(Singleton<Graphics2D>::isInitialized(), "Game was not initialized");

	// Scene collision
	geo::Rectangle viewport = Singleton<Graphics2D>::get()->getCamera().getViewPort();
	float32 yBound = viewport.getHalfHeight();

	if (transform.yPos < -yBound)
	{
		// Powerup fell out of scene
		m_collider->setActive(false);
		m_state = PowerState::IDLE;
		m_vel.x = 0;
		m_vel.y = 0;
	}

	// Integrating new position if powerup still in scene
	if (m_state == PowerState::FALLING)
	{
		transform.xPos += delta * m_vel.x;
		transform.yPos += delta * m_vel.y;
	}
}

bool Powerup::consume()
{
	if (m_state == PowerState::ACTIVE)
	{
		m_state = PowerState::IDLE;
		return true;
	}
	return false;
}

void Powerup::reset()
{
	m_vel.x = 0;
	m_vel.y = 0;
	transform.xPos = 0;
	transform.yPos = 0;
}

void Powerup::render()
{
	if (m_state == PowerState::FALLING)
	{
		GameObject::render();
	}
}

void Powerup::onCollision(const Manifold& manifold, GameObject* collidedWith)
{
	// Check collision with paddle
	Paddle* paddle = dynamic_cast<Paddle*>(collidedWith);
	if (paddle)
	{
		reset();
		m_state = PowerState::ACTIVE;
	}
}


Powerup::~Powerup()
{
}
