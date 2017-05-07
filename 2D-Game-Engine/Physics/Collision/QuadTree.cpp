#include "QuadTree.h"

#include "Core/EngineAssert.h"

#include <utility>

QuadTree::QuadTree(const Rectangle& bounds, uint32 bucketSize) : 
	m_bounds(bounds), 
	m_bucketSize(bucketSize),
	m_northWest(nullptr),
	m_northEast(nullptr),
	m_southWest(nullptr),
	m_southEast(nullptr)
{}

QuadTree::QuadTree(const QuadTree& qt) : m_bounds(qt.m_bounds), m_bucket(qt.m_bucket), m_bucketSize(qt.m_bucketSize) {
	if (qt.isSplit()) {
		m_northWest = new QuadTree(*qt.m_northWest);
		m_northEast = new QuadTree(*qt.m_northEast);
		m_southWest = new QuadTree(*qt.m_southWest);
		m_southEast = new QuadTree(*qt.m_southEast);
	}
	else {
		m_northWest = nullptr;
		m_northEast = nullptr;
		m_southWest = nullptr;
		m_southEast = nullptr;
	}
}

QuadTree::QuadTree(QuadTree&& qt) : m_bounds(qt.m_bounds), m_bucket(qt.m_bucket), m_bucketSize(qt.m_bucketSize) {
	if (qt.isSplit()) {
		m_northWest = qt.m_northWest;
		m_northEast = qt.m_northEast;
		m_southWest = qt.m_southWest;
		m_southEast = qt.m_southEast;
		qt.m_northWest = nullptr;
		qt.m_northEast = nullptr;
		qt.m_southWest = nullptr;
		qt.m_southEast = nullptr;
	}
	else {
		m_northWest = nullptr;
		m_northEast = nullptr;
		m_southWest = nullptr;
		m_southEast = nullptr;
	}
}


QuadTree::~QuadTree() {
	delete m_northWest;
	delete m_northEast;
	delete m_southWest;
	delete m_southEast;
}

void QuadTree::insert(const Rectangle& rect) {
	if (isSplit()) {
		// If has already split
		addToQuadrant(rect, m_bucket);
	}
	else if (m_bucket.size() < m_bucketSize) {
		// If bucket is not already full
		m_bucket.push_back(rect);
	}
	else {
		split();
		addToQuadrant(rect, m_bucket);
	}
}

void QuadTree::retrieve(const Rectangle& rect, std::vector<Rectangle>& list) const {

	for (const Rectangle& rect : m_bucket) {
		list.push_back(rect);
	}

	if (isSplit()) {
		if (rect.intersects(m_northWest->getBounds())) {
			m_northWest->retrieve(rect, list);
		}
		if (rect.intersects(m_northEast->getBounds())) {
			m_northEast->retrieve(rect, list);
		}
		if (rect.intersects(m_southWest->getBounds())) {
			m_southWest->retrieve(rect, list);
		}
		if (rect.intersects(m_southEast->getBounds())) {
			m_southEast->retrieve(rect, list);
		}
	}
}

void QuadTree::split() {
	ASSERT(m_northWest, "Quadtree should never split more than once.");

	float32 quarterWidth = m_bounds.getHalfWidth() / 2.0f;
	float32 quarterHeight = m_bounds.getHalfHeight() / 2.0f;

	m_northWest = new QuadTree(Rectangle(m_bounds.getX() - quarterWidth, m_bounds.getY() + quarterHeight, m_bounds.getHalfWidth(), m_bounds.getHalfHeight()), m_bucketSize);
	m_northEast = new QuadTree(Rectangle(m_bounds.getX() + quarterWidth, m_bounds.getY() + quarterHeight, m_bounds.getHalfWidth(), m_bounds.getHalfHeight()), m_bucketSize);
	m_southWest = new QuadTree(Rectangle(m_bounds.getX() - quarterWidth, m_bounds.getY() - quarterHeight, m_bounds.getHalfWidth(), m_bounds.getHalfHeight()), m_bucketSize);
	m_southEast = new QuadTree(Rectangle(m_bounds.getX() + quarterWidth, m_bounds.getY() - quarterHeight, m_bounds.getHalfWidth(), m_bounds.getHalfHeight()), m_bucketSize);

	std::vector<Rectangle> newBucket;

	for (const Rectangle& rect : m_bucket) {
		addToQuadrant(rect, newBucket);
	}

	m_bucket = std::move(newBucket);
}

void QuadTree::addToQuadrant(const Rectangle& rect, std::vector<Rectangle>& bucket) {
	switch (getQuadrant(rect)) {
	case 0:
		m_northWest->insert(rect);
		break;
	case 1:
		m_northEast->insert(rect);
		break;
	case 2:
		m_southWest->insert(rect);
		break;
	case 3:
		m_southEast->insert(rect);
		break;
	case -1:
		bucket.push_back(rect);
		break;
	default:
		bucket.push_back(rect);
	}
}

int8 QuadTree::getQuadrant(const Rectangle& rect) const {
	if (rect.intersects(m_northWest->getBounds()) && !rect.intersects(m_northEast->getBounds()) && !rect.intersects(m_southWest->getBounds()) && !rect.intersects(m_southEast->getBounds())) {
		return 0;
	}
	else if (!rect.intersects(m_northWest->getBounds()) && rect.intersects(m_northEast->getBounds()) && !rect.intersects(m_southWest->getBounds()) && !rect.intersects(m_southEast->getBounds())) {
		return 1;
	}
	else if (!rect.intersects(m_northWest->getBounds()) && !rect.intersects(m_northEast->getBounds()) && rect.intersects(m_southWest->getBounds()) && !rect.intersects(m_southEast->getBounds())) {
		return 2;
	}
	else if (!rect.intersects(m_northWest->getBounds()) && !rect.intersects(m_northEast->getBounds()) && !rect.intersects(m_southWest->getBounds()) && rect.intersects(m_southEast->getBounds())) {
		return 3;
	}
	else {
		return -1;
	}
}

QuadTree& QuadTree::operator=(const QuadTree& qt) {
	m_bounds = qt.m_bounds;
	m_bucket = qt.m_bucket;
	m_bucketSize = qt.m_bucketSize;

	if (isSplit()) {
		delete m_northWest;
		delete m_northEast;
		delete m_southWest;
		delete m_southEast;
	}
	
	if (qt.isSplit()) {
		m_northWest = new QuadTree(*qt.m_northWest);
		m_northEast = new QuadTree(*qt.m_northEast);
		m_southWest = new QuadTree(*qt.m_southWest);
		m_southEast = new QuadTree(*qt.m_southEast);
	}
	else {
		m_northWest = nullptr;
		m_northEast = nullptr;
		m_southWest = nullptr;
		m_southEast = nullptr;
	}

	return *this;
}

QuadTree& QuadTree::operator=(QuadTree&& qt) {
	m_bounds = qt.m_bounds;
	m_bucket = qt.m_bucket;
	m_bucketSize = qt.m_bucketSize;

	if (isSplit()) {
		delete m_northWest;
		delete m_northEast;
		delete m_southWest;
		delete m_southEast;
	}

	if (qt.isSplit()) {
		m_northWest = qt.m_northWest;
		m_northEast = qt.m_northEast;
		m_southWest = qt.m_southWest;
		m_southEast = qt.m_southEast;
		qt.m_northWest = nullptr;
		qt.m_northEast = nullptr;
		qt.m_southWest = nullptr;
		qt.m_southEast = nullptr;
	}
	else {
		m_northWest = nullptr;
		m_northEast = nullptr;
		m_southWest = nullptr;
		m_southEast = nullptr;
	}

	return *this;
}