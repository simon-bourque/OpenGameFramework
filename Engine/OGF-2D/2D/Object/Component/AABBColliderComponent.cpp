#include "AABBColliderComponent.h"

#include "2D/Object/GameObject.h"

#include "Core/Game.h"
#include "2D/Event.h"

#include "2D/Scene/Scene2D.h"
#include "2D/Scene/SceneManager2D.h"
#include "2D/Physics/Collision/CollisionSystem.h"

#ifdef DEBUG_BUILD
#include "2D/DebugOLD.h"
#include "2D/Graphics/Graphics2D.h"
#include "2D/Graphics/Renderer/ShapeRenderer.h"
#include "Core/Graphics/Color.h"
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
	getSceneManager2DInstance()->getCurrentScene().getCollisionSystem()->addCollider(this);
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
	if (getDebugOLDInstance()->flag("show_colliders")) {
		getGraphics2DInstance()->getShapeRenderer().drawRectangle(m_rectangle, Color::MAGENTA, false);
	}
}
#endif