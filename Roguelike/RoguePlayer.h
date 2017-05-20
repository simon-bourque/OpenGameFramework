#pragma once

#include "Core/Core.h"
#include "Object/GameObject.h"

class RoguePlayer: public GameObject {
private:
	int32 m_currentHealth;
	int32 m_maxHealth;
public:
	RoguePlayer();
	virtual ~RoguePlayer();

	int32 getCurrentHealth() const { return m_currentHealth; };
	int32 getMaxHealth() const { return m_maxHealth; };

	void setCurrentHealth(int32 currentHealth) { m_currentHealth = currentHealth; };
	void setMaxHealth(int32 maxHealth) { m_maxHealth = maxHealth; };
};

