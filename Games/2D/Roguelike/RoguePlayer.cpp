#include "RoguePlayer.h"

#include <iostream>

#include "PlayerController.h"

#include "2D/Scene/SceneManager2D.h"
#include "2D/Scene/Scene2D.h"

#include "2D/Object/Component/CameraComponent.h"
#include "2D/Object/Component/AABBColliderComponent.h"
#include "2D/Object/Component/AnimatorComponent.h"
#include "2D/Object/Component/SpriteComponent.h"

#include "Core/Graphics/TextureCache.h"
#include "Core/Graphics/Texture.h"
#include "Core/Graphics/GraphicsContext.h"
#include "2D/Graphics/Animation/Animation.h"
#include "2D/Graphics/Animation/SpriteSequenceAnimState.h"
#include "2D/Graphics/Animation/SpriteAnimState.h"
#include "2D/Graphics/Graphics2D.h"

RoguePlayer::RoguePlayer() : m_currentHealth(3), m_maxHealth(3), m_bonusAgility(0), m_bonusDamage(0), m_bonusDefense(0), m_isGhost(false), m_bonusTimer(BONUS_DURATION) {

	const static float32 ANIM_DELAY = 0.07f;

	TextureRef tex_walk_s = getGraphicsContextInstance()->getTextureCache()->loadTexture("Rogue_walk_s.tx");
	TextureRef tex_walk_w = getGraphicsContextInstance()->getTextureCache()->loadTexture("Rogue_walk_w.tx");
	TextureRef tex_walk_e = getGraphicsContextInstance()->getTextureCache()->loadTexture("Rogue_walk_e.tx");
	TextureRef tex_walk_n = getGraphicsContextInstance()->getTextureCache()->loadTexture("Rogue_walk_n.tx");

	TextureRef tex_idle_s = getGraphicsContextInstance()->getTextureCache()->loadTexture("Rogue_idle_s.tx");
	TextureRef tex_idle_w = getGraphicsContextInstance()->getTextureCache()->loadTexture("Rogue_idle_w.tx");
	TextureRef tex_idle_e = getGraphicsContextInstance()->getTextureCache()->loadTexture("Rogue_idle_e.tx");
	TextureRef tex_idle_n = getGraphicsContextInstance()->getTextureCache()->loadTexture("Rogue_idle_n.tx");

	uint32 frames[4] = { 0,1,2,3 };
	float32 delays[4] = { ANIM_DELAY, ANIM_DELAY, ANIM_DELAY, ANIM_DELAY };
	Animation walkAnim(frames, delays, 4);

	AnimState* state_walk_s = new SpriteSequenceAnimState(tex_walk_s, walkAnim);
	AnimState* state_walk_w = new SpriteSequenceAnimState(tex_walk_w, walkAnim);
	AnimState* state_walk_e = new SpriteSequenceAnimState(tex_walk_e, walkAnim);
	AnimState* state_walk_n = new SpriteSequenceAnimState(tex_walk_n, walkAnim);

	AnimState* state_idle_s = new SpriteAnimState(tex_idle_s);
	AnimState* state_idle_w = new SpriteAnimState(tex_idle_w);
	AnimState* state_idle_e = new SpriteAnimState(tex_idle_e);
	AnimState* state_idle_n = new SpriteAnimState(tex_idle_n);

	AnimatorComponent* animator = new AnimatorComponent(this, "IDLE_S", state_idle_s);
	animator->addState("IDLE_W", state_idle_w);
	animator->addState("IDLE_E", state_idle_e);
	animator->addState("IDLE_N", state_idle_n);

	animator->addState("WALK_S", state_walk_s);
	animator->addState("WALK_W", state_walk_w);
	animator->addState("WALK_E", state_walk_e);
	animator->addState("WALK_N", state_walk_n);

	addComponent(animator);
	
	addComponent(new AABBColliderComponent(this, geo::Rectangle(1,1)));
	addComponent(new PlayerController(this, this));

	CameraComponent* camera = new CameraComponent(this, &getGraphics2DInstance()->getCamera());
	camera->setSceneBounded(true);
	camera->setSceneBounds(CameraComponent::BOUNDED_ALL);
	addComponent(camera);

	// ################### SWORD #########################
	TextureRef swordTexture = getGraphicsContextInstance()->getTextureCache()->loadTexture("sword.tx");
	m_sword = new GameObject();
	m_sword->transform.scale(0.5f);
	SpriteComponent* spComp = new SpriteComponent(m_sword, swordTexture);
	spComp->setVisible(false);
	m_sword->addComponent(spComp);
	getSceneManager2DInstance()->getCurrentScene().addGameObject(m_sword);
}

void RoguePlayer::tick(float32 delta) {
	GameObject::tick(delta);
	if (m_bonusTimer < 0) {
		m_currentEffect = effect::SOBER;
	}
	else {
		m_bonusTimer -= delta;
	}

	switch (m_currentEffect) {
	case effect::AGI: m_bonusAgility = 2;
		break;
	case effect::CRIPPLE: m_bonusAgility = -2;
		break;
	case effect::DEF: m_bonusDefense = 2;
		break;
	case effect::GHOST: //TODO implement ghost mode
		break;
	case effect::HEALTH: m_currentHealth += 1;
		break;
	case effect::POISON: m_currentHealth -= 1;
		break;
	case effect::STR: m_bonusDamage = 2;
		break;
	case effect::VULN: m_bonusDefense -= 2;
		break;
	case effect::WEAK: m_bonusDamage -= 2;
		break;
	case effect::SOBER:
		m_bonusTimer = BONUS_DURATION;
		m_bonusAgility = 0;
		m_bonusDamage = 0;
		m_bonusDefense = 0;
	}
}

RoguePlayer::~RoguePlayer() {
}
