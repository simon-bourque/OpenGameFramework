#include "WalkState.h"

#include "PlayerController.h"

#include "2D/Object/GameObject.h"

#include "Core/Core.h"
#include "2D/Event.h"

WalkState::WalkState(PlayerController* pc) : PlayerState(pc) {}

WalkState::~WalkState() {}

void WalkState::onEnter() {
	const string str("WALK");
	m_pc->getParentObject()->broadcastEvent(Event(Event::Type::ANIM_STATE_CHANGE, &str));
}

void WalkState::onExit() {}

void WalkState::tick(float32 delta) {
	m_pc->getParentObject()->transform.translate(PlayerController::SPEED * delta * m_pc->getDirection(), 0);
}