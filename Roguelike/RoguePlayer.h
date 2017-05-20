#pragma once

#include "Core/Core.h"
#include "Object/GameObject.h"
#include "Effects.h"

class RoguePlayer: public GameObject {
private:
	float32 const BONUS_DURATION = 10;

	int32 m_currentHealth;
	int32 m_maxHealth;

	int32 m_baseDamage;
	int32 m_baseDefense;
	int32 m_baseAgility;
	int32 m_baseSpeed;
	int32 m_bonusDamage;
	int32 m_bonusDefense;
	int32 m_bonusAgility;
	int32 m_bonusSpeed;

	float32 m_bonusTimer;

	bool m_isGhost;

	effect m_currentEffect;
	
public:
	RoguePlayer();
	virtual ~RoguePlayer();

	int32 getCurrentHealth() const { return m_currentHealth; };
	int32 getMaxHealth() const { return m_maxHealth; };
	uint32 getDamage() const { return m_baseDamage + m_bonusDamage; };
	uint32 getAgility() const { return m_baseAgility + m_bonusAgility; };
	uint32 getDefense() const { return m_baseDefense + m_bonusDefense; };

	void tick(float32 delta) override;

	void setCurrentHealth(int32 currentHealth) { m_currentHealth = currentHealth; };
	void setMaxHealth(int32 maxHealth) { m_maxHealth = maxHealth; };
};

