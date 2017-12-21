#pragma once

#include "Core/Core.h"

#include "2D/Object/Component/ObjectComponent.h"
#include "Core/Delegate.h"
#include "2D/Physics/Collision/Manifold.h"

#include "Math/Geometry/Shape.h"

#include "2D/Object/Component/ComponentType.h"

#include "Math/Geometry/Rectangle.h"

class GameObject;

class AABBColliderComponent : public ObjectComponent {
private:
	typedef Delegate<const Manifold&, GameObject*> CollisionResponse;

	geo::Rectangle m_rectangle;
	CollisionResponse m_response;

	float32 m_xOffset;
	float32 m_yOffset;

	void emptyIntersectResponse(const Manifold& manifold, GameObject* other);
public:
	AABBColliderComponent(GameObject* parentObject, const geo::Rectangle& rectangle, float32 xOffset = 0.0f, float32 yOffset = 0.0f);
	virtual ~AABBColliderComponent();

	virtual void tick(float32 delta) override;

	void resetPosition();

	void onIntersectLevel(const Manifold& manifold);
	void onIntersectObject(const Manifold& manifold, GameObject* other);

	void setCollisionResponse(const Delegate<const Manifold&, GameObject*>& response) { m_response = response; };

	geo::Rectangle getRectangle() const { return m_rectangle; };

	virtual void receiveEvent(const Event& event) override;

#ifdef DEBUG_BUILD
	virtual void debugRender() override;
#endif

	GET_COMPONENT_TYPE_DECLERATION
};

REGISTER_COMPONENT_TYPE(AABBColliderComponent)
