#pragma once
#ifndef QUADTREE_H
#define QUADTREE_H

#include "Core/Core.h"

#include "Math/Geometry/Rectangle.h"

#include <vector>

#ifdef DEBUG_BUILD
	class RenderSystem;
#endif

class QuadTree {
private:
	Rectangle m_bounds;

	std::vector<Rectangle> m_bucket;
	uint32 m_bucketSize;

	QuadTree* m_northWest;
	QuadTree* m_northEast;
	QuadTree* m_southWest;
	QuadTree* m_southEast;

	void split();
	int8 getQuadrant(const Rectangle& rect) const;
	void addToQuadrant(const Rectangle& rect, std::vector<Rectangle>& bucket);
public:
	QuadTree(const Rectangle& bounds, uint32 bucketSize = 1);
	QuadTree(const QuadTree& qt);
	QuadTree(QuadTree&& qt);
	virtual ~QuadTree();

	const Rectangle& getBounds() const { return m_bounds; };
	bool isSplit() const { return m_northWest != nullptr; };

	void insert(const Rectangle& rect);
	void retrieve(const Rectangle& rect, std::vector<Rectangle>& list) const;

	QuadTree& operator=(const QuadTree& qt);
	QuadTree& operator=(QuadTree&& qt);

#ifdef DEBUG_BUILD
	void render() const;
#endif
};

#endif