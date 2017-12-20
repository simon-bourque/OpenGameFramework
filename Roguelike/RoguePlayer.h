#pragma once

#include "Core/Core.h"
#include "2D/Object/GameObject.h"
#include "Effects.h"

class RoguePlayer: public GameObject {
private:
	float32 const BONUS_DURATION = 10;

	int32 m_currentHealth;
	int32 m_maxHealth;

	int32 m_baseDamage;
	int32 m_baseDefense;
	int32 m_baseAgility;
	int32 m_bonusDamage;
	int32 m_bonusDefense;
	int32 m_bonusAgility;

	float32 m_bonusTimer;

	bool m_isGhost;

	effect m_currentEffect;

	GameObject* m_sword;
	
public:
	RoguePlayer();
	virtual ~RoguePlayer();

	int32 getCurrentHealth() const { return m_currentHealth; };
	int32 getMaxHealth() const { return m_maxHealth; };
	int32 getDamage() const { return m_baseDamage + m_bonusDamage; };
	int32 getAgility() const { return m_baseAgility + m_bonusAgility; };
	int32 getDefense() const { return m_baseDefense + m_bonusDefense; };

	void setBonusDefense(int32 defense) { m_bonusDefense = defense; };
	void setBonusDamage(int32 damage) { m_bonusDamage = damage; };
	void setBonusAgility(int32 agility) { m_bonusAgility = agility; };


	GameObject* getSword() { return m_sword; };

	void tick(float32 delta) override;

	void setCurrentHealth(int32 currentHealth) { m_currentHealth = currentHealth; };
	void setMaxHealth(int32 maxHealth) { m_maxHealth = maxHealth; };
};

