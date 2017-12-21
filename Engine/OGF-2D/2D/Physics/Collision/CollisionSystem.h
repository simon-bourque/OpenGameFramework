#pragma once

#include "2D/Physics/Collision/Manifold.h"

#include <vector>

class AABBColliderComponent;
class QuadTree;

namespace geo {
	class Rectangle;
}

class CollisionSystem {
private:
	std::vector<AABBColliderComponent*> m_objects;

	QuadTree* m_tree;

	Manifold generateManifold(const geo::Rectangle& rA, const geo::Rectangle& rB);
public:
	CollisionSystem(const geo::Rectangle& bounds);
	virtual ~CollisionSystem();

	void addCollider(AABBColliderComponent* collider);
	void addStaticCollider(const geo::Rectangle& collider);

	void narrowScan();

	const QuadTree* getQuadTree() const { return m_tree; };
};