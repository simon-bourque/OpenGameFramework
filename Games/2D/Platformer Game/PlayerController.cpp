#include "PlayerController.h"

#include "2D/Event.h"

#include "Core/Input/Input.h"

#include "2D/Object/GameObject.h"

#include "Core/Math/Vector2f.h"

#include "2D/Object/State/StateMachine.h"
#include "2D/Object/State/StateTransition.h"

#include "PlayerState.h"
#include "IdleState.h"
#include "WalkState.h"
#include "InAirState.h"

#ifdef DEBUG_BUILD
#include "NoClipState.h"
#endif

const float32 PlayerController::SPEED = 9.0f;
const float32 PlayerController::JUMP_POWER = 1050.0f;

PlayerController::PlayerController(GameObject* parentObject) : ObjectComponent(parentObject),
	m_leftAction(false),
	m_rightAction(false),
	m_upAction(false),
	m_downAction(false),
	m_toggleFly(false),
	m_canJump(true),
	m_grounded(false),
	m_lastDirection(0)
{
	getInputInstance()->addKeyListener(KeyListener::create<PlayerController, &PlayerController::onKey>(this));

	m_sm = new StateMachine();

	m_sm->createState<IdleState>("player_idle", this);
	m_sm->createState<WalkState>("player_walk", this);
	m_sm->createState<InAirState>("player_in_air", this);

#ifdef DEBUG_BUILD
	m_sm->createState<NoClipState>("player_no_clip", this);
	m_sm->createTransition("idle_to_no_clip", "player_idle", "player_no_clip", StateTransition::createCondition(this, &PlayerController::shouldToggleFly));
	m_sm->createTransition("walk_to_no_clip", "player_walk", "player_no_clip", StateTransition::createCondition(this, &PlayerController::shouldToggleFly));
	m_sm->createTransition("in_air_to_no_clip", "player_in_air", "player_no_clip", StateTransition::createCondition(this, &PlayerController::shouldToggleFly));
	m_sm->createTransition("no_clip_to_idle", "player_no_clip", "player_idle", StateTransition::createCondition(this, &PlayerController::shouldToggleFly));
#endif

	// Idle transitions
	m_sm->createTransition("idle_to_walk", "player_idle", "player_walk", StateTransition::createCondition(this, &PlayerController::isMovingLeftOrRight));
	m_sm->createTransition("idle_to_in_air", "player_idle", "player_in_air", StateTransition::createCondition(this, &PlayerController::isInAir));

	// Walk transitions
	m_sm->createTransition("walk_to_idle", "player_walk", "player_idle", StateTransition::createCondition(this, &PlayerController::isStandingStill));
	m_sm->createTransition("walk_to_in_air", "player_walk", "player_in_air", StateTransition::createCondition(this, &PlayerController::isInAir));

	// In Air transitions
	m_sm->createTransition("in_air_to_idle", "player_in_air", "player_idle", StateTransition::createCondition(this, &PlayerController::isOnGround));

	m_sm->setCurrentState("player_idle");
}


PlayerController::~PlayerController() {
	delete m_sm;
}

void PlayerController::tick(float32 delta) {
	m_sm->tick(delta);

	int32 direction = getDirection();

	if (direction != m_lastDirection) {
		switch (direction) {
		case -1:
			getParentObject()->broadcastEvent(Event(Event::Type::FLIP_SPRITE, true));
			break;
		case 1:
			getParentObject()->broadcastEvent(Event(Event::Type::FLIP_SPRITE, false));
			break;
		default:
			break;
		}
	}

	m_lastDirection = direction;

	m_toggleFly = false;
	m_grounded = false;
}

int32 PlayerController::getDirection() const {
	if (m_leftAction && !m_rightAction) {
		return -1;
	}
	if (!m_leftAction && m_rightAction) {
		return 1;
	}
	return 0;
}

void PlayerController::receiveEvent(const Event& event) {
	switch (event.type) {
	case Event::Type::HIT_GROUND: {
		m_grounded = event.param.asBool;
		break;
	}
	default:
		break;
	}
}

void PlayerController::onKey(int32 key, int32 scancode, int32 action, int32 mods) {
	if (key == Keys::KEY_A) {
		if (action == Actions::PRESS)
			m_leftAction = true;
		if (action == Actions::RELEASE)
			m_leftAction = false;
	}
	if (key == Keys::KEY_D) {
		if (action == Actions::PRESS)
			m_rightAction = true;
		if (action == Actions::RELEASE)
			m_rightAction = false;
	}
	if (key == Keys::KEY_S) {
		if (action == Actions::PRESS)
			m_downAction = true;
		if (action == Actions::RELEASE)
			m_downAction = false;
	}
	if (key == Keys::KEY_W) {
		if (action == Actions::PRESS)
			m_upAction = true;
		if (action == Actions::RELEASE)
			m_upAction = false;
	}
	if (key == Keys::KEY_SPACE && action == Actions::PRESS && m_canJump) {
		static const Vector2f impulse(0, JUMP_POWER);
		getParentObject()->broadcastEvent(Event(Event::Type::APPLY_IMPULSE, &impulse));
		m_grounded = false;
	}
	if (key == Keys::KEY_V) {
		if (action == Actions::PRESS)
			m_toggleFly = true;
		if (action == Actions::RELEASE)
			m_toggleFly = false;
	}
}

#ifdef DEBUG_BUILD
void PlayerController::debugRender() {}
#endif