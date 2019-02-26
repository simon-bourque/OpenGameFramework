#include "Brick.h"
#include "Ball.h"
#include "Level.h"
#include "Powerup.h"

#include "Core/Graphics/GraphicsContext.h"
#include "Core/Graphics/TextureCache.h"

#include "2D/Object/Component/SpriteComponent.h"
#include "2D/Object/Component/AABBColliderComponent.h"

#include <stdlib.h>

Brick::Brick()
	: m_type(BrickType::NORMAL)
	, m_state(BrickState::VISIBLE)
{
	transform.xScale = BRICK_WIDTH;
	transform.yScale = BRICK_HEIGHT;

	TextureRef brickTextureRef = getGraphicsContextInstance()->getTextureCache()->loadTexture("brick.tx");

	SpriteComponent* sprite = new SpriteComponent(this, brickTextureRef);
	addComponent(sprite);

	AABBColliderComponent* collider = new AABBColliderComponent(this, geo::Rectangle(BRICK_WIDTH, BRICK_HEIGHT));
	collider->setCollisionResponse(Delegate<const Manifold&, GameObject*>::create<Brick, &Brick::onCollision>(this));
	addComponent(collider);
}

Brick::Brick(Level* level, BrickType type, Vector2f position)
	: m_level(level)
	, m_position(position)
	, m_type(type)
	, m_state(BrickState::VISIBLE)
{
	transform.xScale = BRICK_WIDTH;
	transform.yScale = BRICK_HEIGHT;

	transform.xPos = m_position.x;
	transform.yPos = m_position.y;

	TextureRef brickTextureRef = getGraphicsContextInstance()->getTextureCache()->loadTexture("brick.tx");

	m_sprite = new SpriteComponent(this, brickTextureRef);
	addComponent(m_sprite);

	m_collider = new AABBColliderComponent(this, geo::Rectangle(BRICK_WIDTH, BRICK_HEIGHT));
	m_collider->setCollisionResponse(Delegate<const Manifold&, GameObject*>::create<Brick, &Brick::onCollision>(this));
	addComponent(m_collider);
}

void Brick::reset()
{
	std::lock_guard<std::mutex> lock(m_mutex);

	m_state = BrickState::VISIBLE;
	m_collider->setActive(true);
	m_sprite->setVisible(true);
}

void Brick::onCollision(const Manifold& manifold, GameObject* collidedWith)
{
	// Check collision with ball
	Ball* ball = dynamic_cast<Ball*>(collidedWith);
	if (ball)
	{
		m_state = BrickState::DESTROYED;
		m_collider->setActive(false);
		m_sprite->setVisible(false);

		if (((rand() % 100) + 1) <= POWERUP_CHANCE)
		{
			m_level->spawnPowerup(m_position);
		}
	}
}

Brick::~Brick()
{
}