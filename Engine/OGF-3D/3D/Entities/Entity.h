#pragma once

#include "3D/Objects/Transform.h"

class Entity
{
public:
	Entity();
	virtual ~Entity();

	Transform transform;

	virtual void update(float32 deltaSeconds);
};
