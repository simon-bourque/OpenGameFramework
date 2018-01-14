#include "RigidBodyComponent.h"

#include "2D/Event.h"

#include "2D/Object/GameObject.h"
#include "2D/Object/Transform.h"

#include "2D/Physics/Collision/Manifold.h"

#include "2D/Scene/SceneManager2D.h"
#include "2D/Scene/Scene2D.h"

GET_COMPONENT_TYPE_DEFINITION(RigidBodyComponent)

RigidBodyComponent::RigidBodyComponent(GameObject* parentObject, float32 mass) : ObjectComponent(parentObject), m_mass(mass) {}


RigidBodyComponent::~RigidBodyComponent() {}

void RigidBodyComponent::applyForce(const Vector2f& force) {
	m_netForce = m_netForce + force;
}

void RigidBodyComponent::applyImpulse(const Vector2f& impulse) {
	m_velocity = m_velocity + (impulse / m_mass);
}

void RigidBodyComponent::stop() {
	m_netForce = { 0, 0 };
	m_velocity = { 0, 0 };
}

void RigidBodyComponent::tick(float32 delta) {

	// Apply Gravity
	applyForce({0, -getSceneManager2DInstance()->getCurrentScene().getGravity() * m_mass});

	Vector2f acceleration(m_netForce / m_mass);

	m_netForce = { 0, 0 };

	Vector2f vF(m_velocity + (acceleration * delta));
	Vector2f displacement(((m_velocity + vF) / 2) * delta);

	m_parentObject->transform.translate(displacement);
	m_velocity = vF;
}

void RigidBodyComponent::receiveEvent(const Event& event) {
	switch (event.type) {
	case Event::Type::APPLY_IMPULSE: {
		const Vector2f* impulse = event.paramAsClass<Vector2f>();
		applyImpulse(*impulse);
		break;
	}
	case Event::Type::COLLISION_LEVEL: {
		const Vector2f& direction = event.paramAsClass<Manifold>()->direction;

		if (direction == Vector2f::Y_AXIS) {
			if (m_velocity.y < 0) {
				m_velocity.y = 0;
			}

			getParentObject()->broadcastEvent(Event(Event::Type::HIT_GROUND, true));
		}
		else if (direction == Vector2f::Y_AXIS.reverse()) {
			if (m_velocity.y > 0) {
				m_velocity.y = 0;
			}
		}

		break;
	}
	default:
		break;
	}
}

#ifdef DEBUG_BUILD
void RigidBodyComponent::debugRender() {}
#endif