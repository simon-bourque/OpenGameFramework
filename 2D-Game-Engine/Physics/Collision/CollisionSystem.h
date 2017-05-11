#pragma once
#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "Physics/Collision/Manifold.h"

#include <vector>

class AABBColliderComponent;
class QuadTree;
class Rectangle;

class CollisionSystem {
private:
	std::vector<AABBColliderComponent*> m_objects;

	QuadTree* m_tree;

	Manifold generateManifold(const Rectangle& rA, const Rectangle& rB);
public:
	CollisionSystem(const Rectangle& bounds);
	virtual ~CollisionSystem();

	void addCollider(AABBColliderComponent* collider);
	void addStaticCollider(const Rectangle& collider);

	void narrowScan();
};

#endif