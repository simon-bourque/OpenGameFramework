#pragma once
#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "Core/Core.h"

#include "Object/Component/ObjectComponent.h"
#include "Core/Delegate.h"
#include "Physics/Collision/Manifold.h"

#include "Math/Geometry/Shape.h"

#include "Object/Component/ComponentType.h"

#include "Math/Geometry/Rectangle.h"

class GameObject;

class AABBColliderComponent : public ObjectComponent {
private:
	typedef BaseDelegate<const Manifold&, GameObject*> CollisionResponse;

	Rectangle m_rectangle;
	CollisionResponse* m_response;

	float32 m_xOffset;
	float32 m_yOffset;

	void emptyIntersectResponse(const Manifold& manifold, GameObject* other);
public:
	AABBColliderComponent(GameObject* parentObject, const Rectangle& rectangle, float32 xOffset = 0.0f, float32 yOffset = 0.0f);
	virtual ~AABBColliderComponent();

	virtual void tick(float32 delta, Game* game) override;

	void resetPosition();

	void onIntersectLevel(const Manifold& manifold);
	void onIntersectObject(const Manifold& manifold, GameObject* other);

	template<typename T>
	void setCollisionResponse(T* object, void(T::*method)(const Manifold& manifold, GameObject* other));

	Rectangle getRectangle() const { return m_rectangle; };

	virtual ComponentType getType() override { return AABB_COLLIDER_COMPONENT; };
	virtual void receiveEvent(const Event& event) override;
};

template<typename T>
void AABBColliderComponent::setCollisionResponse(T* object, void(T::*method)(const Manifold& manifold, GameObject* other)) {
	delete m_response;
	m_response = new Delegate<AABBColliderComponent, const Manifold&, GameObject*>(object, method);
}

#endif