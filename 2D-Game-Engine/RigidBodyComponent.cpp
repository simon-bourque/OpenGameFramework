#include "RigidBodyComponent.h"

#include "GameObject.h"
#include "Transform.h"

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

void RigidBodyComponent::tick(float32 delta, Game* game) {

	// Apply Gravity
	applyForce({0, -9.8f * m_mass});

	Vector2f acceleration(m_netForce / m_mass);

	m_netForce = { 0, 0 };

	Vector2f vF(m_velocity + (acceleration * delta));
	Vector2f displacement(((m_velocity + vF) / 2) * delta);

	m_parentObject->getTransform().translate(displacement);
	m_velocity = vF;
}