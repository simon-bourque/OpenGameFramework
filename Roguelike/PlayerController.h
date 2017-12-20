#pragma once

#include "2D/Object/Component/ObjectComponent.h"

#include "Core/Core.h"

class RoguePlayer;

class PlayerController : public ObjectComponent {
private:
	enum Direction {
		NORTH,
		NORTH_WEST,
		NORTH_EAST,
		SOUTH,
		SOUTH_WEST,
		SOUTH_EAST,
		EAST,
		WEST,
		NONE
	};

	Direction m_lastDirection;

	bool m_upAction;
	bool m_downAction;
	bool m_leftAction;
	bool m_rightAction;

	Direction getDirection() const;

	RoguePlayer* m_player;
	float32 m_swordOffset;
	float32 m_swordCountdown;
	float32 m_walkDelay;
	bool m_swingingSword;

	void flipSwordSprite(Direction direction);
	void selectAnimation(Direction direction);
	void movePlayer(Direction direction, float32 delta);
public:
	PlayerController(GameObject* parentObject, RoguePlayer* player);
	virtual ~PlayerController();

	virtual void tick(float32 delta) override;
	virtual void receiveEvent(const Event& event) override;

	void onKey(int32 key, int32 scancode, int32 action, int32 mods);

#ifdef DEBUG_BUILD
	virtual void debugRender() override;
#endif
};

