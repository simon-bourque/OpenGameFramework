#include "Manifold.h"


Manifold::Manifold() : 
	colA(nullptr),
	colB(nullptr),
	depth(0)
{}

Manifold::Manifold(Shape* colA, Shape* colB, float32 depth, const Vector2f& direction) :
	colA(colA),
	colB(colB),
	depth(depth),
	direction(direction)
{}


Manifold::~Manifold() {}
