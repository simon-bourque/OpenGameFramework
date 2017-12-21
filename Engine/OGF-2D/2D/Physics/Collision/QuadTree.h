#pragma once

#include "Core/Core.h"

#include "Math/Geometry/Rectangle.h"

#include <vector>

#ifdef DEBUG_BUILD
	class RenderSystem;
#endif

class QuadTree {
private:
	geo::Rectangle m_bounds;

	std::vector<geo::Rectangle> m_bucket;
	uint32 m_bucketSize;

	QuadTree* m_northWest;
	QuadTree* m_northEast;
	QuadTree* m_southWest;
	QuadTree* m_southEast;

	void split();
	int8 getQuadrant(const geo::Rectangle& rect) const;
	void addToQuadrant(const geo::Rectangle& rect, std::vector<geo::Rectangle>& bucket);
public:
	QuadTree(const geo::Rectangle& bounds, uint32 bucketSize = 1);
	QuadTree(const QuadTree& qt);
	QuadTree(QuadTree&& qt);
	virtual ~QuadTree();

	const geo::Rectangle& getBounds() const { return m_bounds; };
	bool isSplit() const { return m_northWest != nullptr; };

	void insert(const geo::Rectangle& rect);
	void retrieve(const geo::Rectangle& rect, std::vector<geo::Rectangle>& list) const;

	QuadTree& operator=(const QuadTree& qt);
	QuadTree& operator=(QuadTree&& qt);

#ifdef DEBUG_BUILD
	void render() const;
#endif
};