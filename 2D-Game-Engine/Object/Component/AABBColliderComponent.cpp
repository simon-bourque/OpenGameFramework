#include "AABBColliderComponent.h"

#include "Object/GameObject.h"

#include "Core/Game.h"
#include "Core/Event.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Physics/Collision/CollisionSystem.h"

#ifdef DEBUG_BUILD
#include "Core/Debug.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/Color.h"
#include "Graphics/Renderer/ShapeRenderer.h"
#endif

GET_COMPONENT_TYPE_DEFINITION(AABBColliderComponent)

void AABBColliderComponent::emptyIntersectResponse(const Manifold& manifold, GameObject* other) {}

AABBColliderComponent::AABBColliderComponent(GameObject* parentObject, const geo::Rectangle& rectangle, float32 xOffset, float32 yOffset) : ObjectComponent(parentObject),
	m_rectangle(rectangle),
	m_xOffset(xOffset),
	m_yOffset(yOffset)
{
	m_response.bind<AABBColliderComponent, &AABBColliderComponent::emptyIntersectResponse>(this);
}


AABBColliderComponent::~AABBColliderComponent() {}

void AABBColliderComponent::tick(float32 delta) {
	resetPosition();
	getSceneManagerInstance()->getCurrentScene().getCollisionSystem()->addCollider(this);
}

void AABBColliderComponent::resetPosition() {
	const Transform& t = getParentObject()->transform;

	m_rectangle.setX(t.xPos + m_xOffset);
	m_rectangle.setY(t.yPos + m_yOffset);
}

void AABBColliderComponent::onIntersectLevel(const Manifold& manifold) {
	
	Vector2f delta = manifold.direction * manifold.depth;
	getParentObject()->transform.translate(delta.x, delta.y);
	getParentObject()->broadcastEvent(Event(Event::Type::COLLISION_LEVEL, static_cast<const void*>(&manifold)));
}

void AABBColliderComponent::onIntersectObject(const Manifold& manifold, GameObject* other) {
	m_response(manifold, other);
}

void AABBColliderComponent::receiveEvent(const Event& event) {}

#ifdef DEBUG_BUILD
void AABBColliderComponent::debugRender() {
	if (getDebugInstance()->flag("show_colliders")) {
		getRenderSystemInstance()->getShapeRenderer()->drawRectangle(m_rectangle, Color::MAGENTA, false);
	}
}
#endif