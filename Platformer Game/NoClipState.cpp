#ifdef DEBUG_BUILD

#include "NoClipState.h"

#include "Core/Event.h"

#include "PlayerController.h"

#include "Object/GameObject.h"
#include "Object/Component/ComponentType.h"
#include "Object/Component/RigidBodyComponent.h"
#include "Object/Component/CameraComponent.h"

NoClipState::NoClipState(PlayerController* pc) : PlayerState(pc) {}


NoClipState::~NoClipState() {}

void NoClipState::onEnter() {
	const string str("IDLE");
	m_pc->getParentObject()->broadcastEvent(Event(Event::Type::ANIM_STATE_CHANGE, &str));

	ObjectComponent* rigidBody = m_pc->getParentObject()->findComponent(RIGIDBODY_COMPONENT);
	if (rigidBody) {
		rigidBody->setActive(false);
	}

	ObjectComponent* collider = m_pc->getParentObject()->findComponent(AABB_COLLIDER_COMPONENT);
	if (collider) {
		collider->setActive(false);
	}

	ObjectComponent* camera = m_pc->getParentObject()->findComponent(CAMERA_COMPONENT);
	if (camera) {
		static_cast<CameraComponent*>(camera)->setSceneBounded(false);
	}
}

void NoClipState::onExit() {
	ObjectComponent* rigidBody = m_pc->getParentObject()->findComponent(RIGIDBODY_COMPONENT);
	if (rigidBody) {
		rigidBody->setActive(true);
		static_cast<RigidBodyComponent*>(rigidBody)->stop();
	}

	ObjectComponent* collider = m_pc->getParentObject()->findComponent(AABB_COLLIDER_COMPONENT);
	if (collider) {
		collider->setActive(true);
	}

	ObjectComponent* camera = m_pc->getParentObject()->findComponent(CAMERA_COMPONENT);
	if (camera) {
		static_cast<CameraComponent*>(camera)->setSceneBounded(true);
	}
}

void NoClipState::tick(float32 delta) {
	static const float32 SPEED = 20;

	if (m_pc->isLeftAction()) {
		m_pc->getParentObject()->getTransform().translate(-SPEED * delta, 0);
	}
	if (m_pc->isRightAction()) {
		m_pc->getParentObject()->getTransform().translate(SPEED * delta, 0);
	}
	if (m_pc->isUpAction()) {
		m_pc->getParentObject()->getTransform().translate(0, SPEED * delta);
	}
	if (m_pc->isDownAction()) {
		m_pc->getParentObject()->getTransform().translate(0, -SPEED * delta);
	}
}

#endif