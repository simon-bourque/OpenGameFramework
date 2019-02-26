#include "PaddleController.h"
#include "GameObjects/Paddle.h"

#include "Core/Input/Input.h"
#include "Core/Math/Geometry/Rectangle.h"

#include "2D/Object/GameObject.h"
#include "2D/Graphics/Graphics2D.h"

PaddleController::PaddleController(GameObject* parentObject)
	: ObjectComponent(parentObject)
{
	getInputInstance()->addKeyListener(KeyListener::create<PaddleController, &PaddleController::onKey>(this));
}

void PaddleController::tick(float32 delta)
{
	OGF_ASSERT(Singleton<Graphics2D>::isInitialized(), "Game was not initialized");

	// Scene collision
	geo::Rectangle viewport = Singleton<Graphics2D>::get()->getCamera().getViewPort();
	float32 xBound = viewport.getHalfWidth();
	float32 yBound = viewport.getHalfHeight();

	GameObject* paddle = getParentObject();

	if (m_goingLeft && (paddle->transform.xPos - Paddle::PADDLE_WIDTH / 2) > -xBound)
	{
		paddle->transform.xPos -= delta*SHIMMY_RATE;
	}
	if (m_goingRight && (paddle->transform.xPos + Paddle::PADDLE_WIDTH / 2) < xBound)
	{
		paddle->transform.xPos += delta*SHIMMY_RATE;
	}
}

void PaddleController::receiveEvent(const Event& event)
{

}

void PaddleController::onKey(int32 key, int32 scancode, int32 action, int32 mods)
{
	if (key == Keys::KEY_A) {
		if (action == Actions::PRESS)
			m_goingLeft = true;
		if (action == Actions::RELEASE)
			m_goingLeft = false;
	}
	if (key == Keys::KEY_D) {
		if (action == Actions::PRESS)
			m_goingRight = true;
		if (action == Actions::RELEASE)
			m_goingRight = false;
	}
}

#ifdef DEBUG_BUILD
void PaddleController::debugRender()
{
}
#endif

PaddleController::~PaddleController()
{
}
