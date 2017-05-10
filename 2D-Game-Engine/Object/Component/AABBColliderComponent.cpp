#include "AABBColliderComponent.h"

#include "Object/GameObject.h"

void AABBColliderComponent::emptyIntersectResponse(const Manifold& manifold, GameObject* other) {}

AABBColliderComponent::AABBColliderComponent(GameObject* parentObject, const Rectangle& rectangle, float32 xOffset, float32 yOffset) : ObjectComponent(parentObject),
	m_rectangle(rectangle),
	m_xOffset(xOffset),
	m_yOffset(yOffset)
{
	m_response = new Delegate<AABBColliderComponent, const Manifold&, GameObject*>(this, &AABBColliderComponent::emptyIntersectResponse);
}


AABBColliderComponent::~AABBColliderComponent() {
	delete m_response;
}

void AABBColliderComponent::tick(float32 delta, Game* game) {
	resetPosition();
	//game->...
}

void AABBColliderComponent::resetPosition() {
	const Transform& t = getParentObject().getTransform();

	m_rectangle.setX(t.xPos + m_xOffset);
	m_rectangle.setY(t.yPos + m_yOffset);
}

void AABBColliderComponent::onIntersectLevel(const Manifold& manifold) {
	
	Vector2f delta = manifold.direction * manifold.depth;
	getParentObject().getTransform().translate(delta.x, delta.y);
	//getParentObject().broadcastMessage("COLLISION_LEVEL", manifold);
}

void AABBColliderComponent::onIntersectObject(const Manifold& manifold, GameObject* other) {
	(*m_response)(manifold, other);
}