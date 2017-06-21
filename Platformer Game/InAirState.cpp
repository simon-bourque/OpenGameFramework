#include "InAirState.h"

#include "PlayerController.h"

#include "Object/GameObject.h"

#include "Core/Core.h"
#include "Core/Event.h"


InAirState::InAirState(PlayerController* pc) : PlayerState(pc) {}

InAirState::~InAirState() {}

void InAirState::onEnter() {
	const string str("AIR");
	m_pc->getParentObject()->broadcastEvent(Event(Event::Type::ANIM_STATE_CHANGE, &str));
	m_pc->setCanJump(false);
}

void InAirState::onExit() {
	m_pc->setCanJump(true);
}

void InAirState::tick(float32 delta) {
	m_pc->getParentObject()->transform.translate(PlayerController::SPEED * delta * m_pc->getDirection(), 0);
}