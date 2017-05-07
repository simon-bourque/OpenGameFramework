#pragma once
#ifndef OBJECTCOMPONENT_H
#define OBJECTCOMPONENT_H

#include "Core/Core.h"

class GameObject;
class Game;

class ObjectComponent
{
private:
	bool m_active;
protected:
	GameObject* m_parentObject;
public:
	ObjectComponent(GameObject* parentObject);
	virtual ~ObjectComponent();

	const GameObject& getParentObject() const { return *m_parentObject; };
	GameObject& getParentObject() { return *m_parentObject; };

	bool isActive() const { return m_active; };
	void setActive(bool active) { m_active = active; };

	virtual void tick(float32 delta, Game* game) = 0;
};

#endif