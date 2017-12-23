#include "CollisionSystem.h"

#include "Core/Math/Vector2f.h"

#include "2D/Physics/Collision/QuadTree.h"
#include "2D/Object/Component/AABBColliderComponent.h"

#include <cmath>

CollisionSystem::CollisionSystem(const geo::Rectangle& bounds) {
	m_tree = new QuadTree(bounds);
}


CollisionSystem::~CollisionSystem() {
	delete m_tree;
}

void CollisionSystem::addCollider(AABBColliderComponent* collider) {
	m_objects.push_back(collider);
}

void CollisionSystem::addStaticCollider(const geo::Rectangle& collider) {
	m_tree->insert(collider);
}

void CollisionSystem::narrowScan() {
	for (uint32 i = 0; i < m_objects.size(); i++) {
		
		AABBColliderComponent* collider = m_objects[i];
		geo::Rectangle rA = collider->getRectangle();

		for (uint32 j = i + 1; j < m_objects.size(); j++) {
			AABBColliderComponent* other = m_objects[j];
			geo::Rectangle rB = collider->getRectangle();

			if (rA.intersects(rB)) {
				Manifold manifold = generateManifold(rA, rB);
				collider->onIntersectObject(manifold, other->getParentObject());
				other->onIntersectObject(manifold, collider->getParentObject());
			}
		}

		std::vector<geo::Rectangle> rects;
		m_tree->retrieve(rA, rects);

		for (const geo::Rectangle& rect : rects) {
			if (rect.intersects(rA)) {
				collider->onIntersectLevel(generateManifold(rA, rect));
			}
		}
	}

	m_objects.clear();
}

Manifold CollisionSystem::generateManifold(const geo::Rectangle& rA, const geo::Rectangle& rB) {
	Vector2f direction;
	float32 depth = 0;

	// Calculate distance between rectangles
	//Vector2f dist = new Vector2f(rB.getX(), rB.getY()).subtract(new Vector2f(rA.getX(), rA.getY()));
	Vector2f dist = Vector2f(rB.getX(), rB.getY()) - Vector2f(rA.getX(), rA.getY());

	float32 halfWidthA = rA.getWidth() / 2.0f;
	float32 halfWidthB = rB.getWidth() / 2.0f;

	// Calculate overlap in x
	float32 xOverlap = halfWidthA + halfWidthB - abs(dist.x);

	if (xOverlap > 0) {

		float32 halfHeightA = rA.getHeight() / 2.0f;
		float32 halfHeightB = rB.getHeight() / 2.0f;

		// Calculate overlap in y
		float32 yOverlap = halfHeightA + halfHeightB - abs(dist.y);


		// TODO these checks arent necessary if I i already know they're overlapping
		if (yOverlap > 0) {


			if (xOverlap < yOverlap) {

				if (dist.x < 0) {
					direction = { 1, 0 };
				}
				else {
					direction = { -1, 0 };
				}
				depth = xOverlap;

			}
			else {
				if (dist.y < 0) {
					direction = { 0, 1 };
				}
				else {
					direction = { 0, -1 };
				}
				depth = yOverlap;
			}

		}

	}

	return Manifold((Shape*)(&rA), (Shape*)(&rB), depth, direction);
}