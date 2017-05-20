#pragma once

#include "Object/Component/ObjectComponent.h"

class RoguePlayer;

class PlayerController : public ObjectComponent {
private:
	enum Direction {
		NORTH,
		SOUTH,
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
	bool m_swingingSword;
public:
	PlayerController(GameObject* parentObject, RoguePlayer* player);
	virtual ~PlayerController();

	virtual void tick(float32 delta) override;
	virtual ComponentType getType() override;
	virtual void receiveEvent(const Event& event) override;

	void onKey(int32 key, int32 scancode, int32 action, int32 mods);

#ifdef DEBUG_BUILD
	virtual void debugRender() override;
#endif
};

