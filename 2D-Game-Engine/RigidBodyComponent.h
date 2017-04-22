#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include "ObjectComponent.h"

#include "Core.h"
#include "Vector2f.h"

class GameObject;

class RigidBodyComponent : public ObjectComponent {
private:
	float32 m_mass;

	Vector2f m_netForce;
	Vector2f m_velocity;
public:
	RigidBodyComponent(GameObject* parentObject, float32 mass);
	virtual ~RigidBodyComponent();

	void applyForce(const Vector2f& force);
	void applyImpulse(const Vector2f& impulse);
	void stop();

	virtual void tick(float32 delta, Game* game) override;
};

#endif