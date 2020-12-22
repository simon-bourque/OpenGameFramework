#pragma once

#include "Core/Math/Vector2f.h"

#include "2D/Object/Component/ObjectComponent.h"

class PaddleController : public ObjectComponent
{
public:
	PaddleController(GameObject* parentObject);
	virtual ~PaddleController();
	PaddleController(const PaddleController& other) = delete;
	PaddleController(PaddleController&& other) = delete;
	PaddleController& operator=(const PaddleController& other) = delete;
	PaddleController& operator=(PaddleController&& other) = delete;

	virtual void tick(float32 delta) override;
	virtual void receiveEvent(const Event& event) override;

	void onKey(int32 key, int32 scancode, int32 action, int32 mods);

	bool isGoingLeft() const { return m_goingLeft; };
	bool isGoingRight() const { return m_goingRight; };

#ifdef DEBUG_BUILD
	virtual void debugRender() override;
#endif

	// Movement rate of the paddle
	constexpr static float32 SHIMMY_RATE = 40.0f;

private:

	// Paddle states
	bool m_goingLeft;
	bool m_goingRight;
};

