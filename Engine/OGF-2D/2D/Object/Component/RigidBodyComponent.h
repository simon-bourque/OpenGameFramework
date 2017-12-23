#pragma once

#include "Core/Core.h"

#include "2D/Object/Component/ObjectComponent.h"

#include "Core/Math/Vector2f.h"

#include "2D/Object/Component/ComponentType.h"

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

	virtual void tick(float32 delta) override;

	virtual void receiveEvent(const Event& event) override;

#ifdef DEBUG_BUILD
	virtual void debugRender() override;
#endif

	GET_COMPONENT_TYPE_DECLERATION
};

REGISTER_COMPONENT_TYPE(RigidBodyComponent)