#include "GameObjects/Ball.h"
#include "GameObjects/Paddle.h"
#include "GameObjects/Components/PaddleController.h"
#include "GameObjects/Brick.h"
#include "GameObjects/Powerup.h"

#include "Core/Assert.h"
#include "Core/Graphics/GraphicsContext.h"
#include "Core/Graphics/TextureCache.h"
#include "Core/Delegate.h"

#include "2D/Graphics/Graphics2D.h"
#include "2D/Object/Component/SpriteComponent.h"
#include "2D/Object/Component/AABBColliderComponent.h"
#include "2D/Object/Component/EmitterComponent.h"
#include "2D/Physics/Collision/Manifold.h"

#include <algorithm>

#undef min
#undef max

Ball::Ball()
	: m_vel(0.0f, 0.0f)
	, m_docked(true)
	, m_outOfBounds(false)
	, m_xBallMode(false)
	, m_hasFlipped(false)
{
	transform.xScale = BALL_WIDTH;
	transform.yScale = BALL_HEIGHT;

	transform.scale(BALL_SCALE);

	TextureRef ballTextureRef = getGraphicsContextInstance()->getTextureCache()->loadTexture("ball.tx");
	TextureRef xBallTextureRef = getGraphicsContextInstance()->getTextureCache()->loadTexture("xball_particle.tx");


	m_collider = new AABBColliderComponent(this, geo::Rectangle(BALL_WIDTH, BALL_HEIGHT));

	m_collider->setCollisionResponse(Delegate<const Manifold&, GameObject*>::create<Ball, &Ball::onCollision>(this));
	addComponent(m_collider);

	m_emitter = new EmitterComponent(
		this,
		xBallTextureRef,
		"particle_vert.shader",
		"particle_frag.shader");
	m_emitter->setupEmitter(20, 0.25f);
	m_emitter->setVisible(false);
	addComponent(m_emitter);

	m_sprite = new SpriteComponent(this, ballTextureRef);
	addComponent(m_sprite);
}

void Ball::onCollision(const Manifold& manifold, GameObject* collidedWith)
{
	// Check collision with paddle
	Paddle* paddle = dynamic_cast<Paddle*>(collidedWith);
	if (paddle)
	{
		pushBack(manifold);

		// Reflection of the Y velocity of the ball
		if (!m_hasFlipped)
		{
			m_vel.y = abs(m_vel.y);
			m_hasFlipped = true;
		}

		if (!paddle->getController()->isGoingLeft() && !paddle->getController()->isGoingRight())
		{
			// Paddle is not moving, no need to keep going
			return;
		}

		// We are colliding with the paddle
		int32 averageX = (manifold.contactPoints.at(0).x + manifold.contactPoints.at(1).x) / 2;
		int32 deltaX = paddle->transform.xPos - averageX;

		// Ball hit the paddle on the left
		if (paddle->getController()->isGoingLeft())
		{
			m_vel.x -= PaddleController::SHIMMY_RATE * FORCE_CONSTANT;
			m_vel.x = std::max(m_vel.x, -MAX_VEL); // Clamping velocity
		}
		else if (paddle->getController()->isGoingRight())
		{
			m_vel.x += PaddleController::SHIMMY_RATE * FORCE_CONSTANT;
			m_vel.x = std::min(m_vel.x, MAX_VEL); // Clamping velocity
		}

		return;
	}
	
	// Check collision with brick
	Brick* brick = dynamic_cast<Brick*>(collidedWith);
	if (brick && !m_hasFlipped)
	{
		pushBack(manifold);

		if (!m_xBallMode)
		{
			m_vel.y *= -1;
			m_hasFlipped = true;
		}
		return;
	}
}

void Ball::pushBack(const Manifold& manifold)
{
	// Pushing the ball out of the collider
	Vector2f delta = manifold.direction * manifold.minDepth;
	transform.translate(delta.x, delta.y);
}

void Ball::tick(float32 delta)
{
	if (m_xBallMode)
	{
		m_timer += delta;
		if (m_timer >= XBALL_TIMER)
		{
			m_timer = 0.0f;
			activateNormalMode();
		}
	}

	m_hasFlipped = false;

	GameObject::tick(delta);

	OGF_ASSERT(Singleton<Graphics2D>::isInitialized(), "Game was not initialized");

	// Scene collision
	geo::Rectangle viewport = Singleton<Graphics2D>::get()->getCamera().getViewPort();
	float32 xBound = viewport.getHalfWidth();
	float32 yBound = viewport.getHalfHeight();

	if (transform.xPos <= -xBound)
	{
		// Left  bounds
		transform.translate((-xBound - transform.xPos), 0.0f);
		m_vel.x *= -1;
	}
	else if (transform.xPos >= xBound)
	{
		// Right bound
		transform.translate((xBound - transform.xPos), 0.0f);
		m_vel.x *= -1;
	}
	if (transform.yPos >= yBound )
	{
		// Up bound
		transform.translate(0.0f, (yBound - transform.yPos));
		m_vel.y *= -1;
	}
	else if (transform.yPos <= -yBound)
	{
		//Game Over
		m_outOfBounds = true;
	}

	// Integrating new position
	transform.xPos += delta * m_vel.x;
	transform.yPos += delta * m_vel.y;
}

void Ball::setDocked(bool docked)
{
	if (docked)
	{
		m_outOfBounds = false;
		m_vel = { 0.0f, 0.0f};
	}
	else if(m_docked && !docked)
	{
		m_vel.y += UNDOCKING_VEL;
	}

	m_docked = docked;
}

void Ball::activateXBallMode()
{
	m_xBallMode = true;
	m_emitter->setVisible(true);
	m_sprite->replaceTexture("xball.tx");
}

void Ball::activateNormalMode()
{
	m_xBallMode = false;
	m_emitter->setVisible(false);
	m_sprite->replaceTexture("ball.tx");
}

Ball::~Ball()
{
}
