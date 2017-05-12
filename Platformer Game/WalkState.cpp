#include "WalkState.h"

#include "PlayerController.h"

#include "Object/GameObject.h"

#include "Core/Core.h"
#include "Core/Event.h"

WalkState::WalkState(PlayerController* pc) : PlayerState(pc) {}

WalkState::~WalkState() {}

void WalkState::onEnter() {
	const string str("WALK");
	m_pc->getParentObject()->broadcastEvent(Event(Event::Type::ANIM_STATE_CHANGE, &str));
}

void WalkState::onExit() {}

void WalkState::tick(float32 delta, Game* game) {
	m_pc->getParentObject()->getTransform().translate(PlayerController::SPEED * delta * m_pc->getDirection(), 0);
}