#include "Paddle.h"

#include "GameObjects/Components/PaddleController.h"

#include "Core/Graphics/GraphicsContext.h"
#include "Core/Graphics/TextureCache.h"

#include "2D/Object/Component/SpriteComponent.h"
#include "2D/Object/Component/AABBColliderComponent.h"

Paddle::Paddle()
{
	transform.yScale = PADDLE_HEIGHT;
	transform.xScale = PADDLE_WIDTH;
	transform.yPos = PADDLE_YOFFSET;

	TextureRef padTextureRef = getGraphicsContextInstance()->getTextureCache()->loadTexture("paddle.tx");

	m_controller = new PaddleController(this);
	addComponent(m_controller);

	SpriteComponent* sprite = new SpriteComponent(this, padTextureRef);
	addComponent(sprite);

	AABBColliderComponent* collider = new AABBColliderComponent(this, geo::Rectangle(PADDLE_WIDTH, PADDLE_HEIGHT));
	addComponent(collider);
}

Vector2f Paddle::getPosition() const
{
	return { transform.xPos, transform.yPos };
}

Paddle::~Paddle()
{
}