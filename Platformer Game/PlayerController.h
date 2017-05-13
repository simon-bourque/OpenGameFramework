#pragma once
#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "Core/Core.h"

#include "Object/Component/ObjectComponent.h"

class Game;
class GameObject;
struct Event;
class StateMachine;

class PlayerController : public ObjectComponent {\
public:
	static const float32 SPEED;
	static const float32 JUMP_POWER;
private:
	StateMachine* m_sm;

	bool m_leftAction;
	bool m_rightAction;
	bool m_upAction;
	bool m_downAction;

	bool m_toggleFly;

	bool m_canJump;
	bool m_grounded;

	int32 m_lastDirection;

	bool isOnGround() { return m_grounded; };
	bool isInAir() { return !m_grounded; };
public:
	PlayerController(GameObject* parentObject);
	virtual ~PlayerController();

	void tick(float32 delta) override;
	void receiveEvent(const Event& event) override;

	int32 getDirection() const;
	bool isLeftAction() const { return m_leftAction; };
	bool isRightAction() const { return m_rightAction; };
	bool isUpAction() const { return m_upAction; };
	bool isDownAction() const { return m_downAction; };
	bool isMovingLeftOrRight() { return m_leftAction != m_rightAction; };
	bool isStandingStill() { return getDirection() == 0; };
	void setCanJump(bool canJump) { m_canJump = canJump; };

	void onKey(int32 key, int32 scancode, int32 action, int32 mods);
	virtual ComponentType getType() override { return DEFAULT; };

#ifdef DEBUG_BUILD
	virtual void debugRender() override;
#endif
};

#endif