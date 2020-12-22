#include "CollisionSystem.h"

#include "Core/Math/Vector2f.h"

#include "2D/Physics/Collision/QuadTree.h"
#include "2D/Object/Component/AABBColliderComponent.h"

#include <cmath>
#include <algorithm>

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
			geo::Rectangle rB = other->getRectangle();

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
	Manifold manifold;
	manifold.colA = (Shape*)&rA;
	manifold.colB = (Shape*)&rB;

	// Calculate distance between rectangles
	//Vector2f dist = new Vector2f(rB.getX(), rB.getY()).subtract(new Vector2f(rA.getX(), rA.getY()));
	Vector2f dist = Vector2f(rB.getX(), rB.getY()) - Vector2f(rA.getX(), rA.getY());

	float32 halfWidthA = rA.getWidth() / 2.0f;
	float32 halfWidthB = rB.getWidth() / 2.0f;

	// Calculate overlap in x
	float32 xOverlap = halfWidthA + halfWidthB - abs(dist.x);

	if (xOverlap > 0) {
		manifold.depth.x = xOverlap;

		float32 halfHeightA = rA.getHeight() / 2.0f;
		float32 halfHeightB = rB.getHeight() / 2.0f;

		// Calculate overlap in y
		float32 yOverlap = halfHeightA + halfHeightB - abs(dist.y);

		// TODO these checks arent necessary if I i already know they're overlapping
		if (yOverlap > 0) {
			Vector2f c1, c2;
			c1.x = std::max(rA.getMin().x, rB.getMin().x);
			c1.y = std::max(rA.getMin().y, rB.getMin().y);
			c2.x = std::min(rA.getMax().x, rB.getMax().x);
			c2.y = std::min(rA.getMax().y, rB.getMax().y);

			manifold.contactPoints.push_back(c1);
			manifold.contactPoints.push_back(c2);

			manifold.depth.y = yOverlap;

			if (xOverlap < yOverlap) {

				if (dist.x < 0) {
					manifold.direction = { 1, 0 };
				}
				else {
					manifold.direction = { -1, 0 };
				}
				manifold.minDepth = xOverlap;

			}
			else {
				if (dist.y < 0) {
					manifold.direction = { 0, 1 };
				}
				else {
					manifold.direction = { 0, -1 };
				}
				manifold.minDepth = yOverlap;
			}

		}

	}

	return manifold;
}