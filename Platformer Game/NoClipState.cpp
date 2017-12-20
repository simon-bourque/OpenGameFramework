#ifdef DEBUG_BUILD

#include "NoClipState.h"

#include "Core/Event.h"

#include "PlayerController.h"

#include "2D/Object/GameObject.h"
#include "2D/Object/Component/ComponentType.h"
#include "2D/Object/Component/RigidBodyComponent.h"
#include "2D/Object/Component/AABBColliderComponent.h"
#include "2D/Object/Component/CameraComponent.h"

NoClipState::NoClipState(PlayerController* pc) : PlayerState(pc) {}


NoClipState::~NoClipState() {}

void NoClipState::onEnter() {
	const string str("IDLE");
	m_pc->getParentObject()->broadcastEvent(Event(Event::Type::ANIM_STATE_CHANGE, &str));

	RigidBodyComponent* rigidBody = m_pc->getParentObject()->findComponent<RigidBodyComponent>();
	if (rigidBody) {
		rigidBody->setActive(false);
	}

	AABBColliderComponent* collider = m_pc->getParentObject()->findComponent<AABBColliderComponent>();
	if (collider) {
		collider->setActive(false);
	}

	CameraComponent* camera = m_pc->getParentObject()->findComponent<CameraComponent>();
	if (camera) {
		camera->setSceneBounded(false);
	}
}

void NoClipState::onExit() {
	RigidBodyComponent* rigidBody = m_pc->getParentObject()->findComponent<RigidBodyComponent>();
	if (rigidBody) {
		rigidBody->setActive(true);
		rigidBody->stop();
	}

	AABBColliderComponent* collider = m_pc->getParentObject()->findComponent<AABBColliderComponent>();
	if (collider) {
		collider->setActive(true);
	}

	CameraComponent* camera = m_pc->getParentObject()->findComponent<CameraComponent>();
	if (camera) {
		camera->setSceneBounded(true);
	}
}

void NoClipState::tick(float32 delta) {
	static const float32 SPEED = 20;

	if (m_pc->isLeftAction()) {
		m_pc->getParentObject()->transform.translate(-SPEED * delta, 0);
	}
	if (m_pc->isRightAction()) {
		m_pc->getParentObject()->transform.translate(SPEED * delta, 0);
	}
	if (m_pc->isUpAction()) {
		m_pc->getParentObject()->transform.translate(0, SPEED * delta);
	}
	if (m_pc->isDownAction()) {
		m_pc->getParentObject()->transform.translate(0, -SPEED * delta);
	}
}

#endif