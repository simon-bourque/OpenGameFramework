#include "RoguePlayer.h"

#include <iostream>

#include "PlayerController.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

#include "Object/Component/CameraComponent.h"
#include "Object/Component/AABBColliderComponent.h"
#include "Object/Component/AnimatorComponent.h"
#include "Object/Component/SpriteComponent.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/Animation/Animation.h"
#include "Graphics/Animation/SpriteSequenceAnimState.h"
#include "Graphics/Animation/SpriteAnimState.h"

RoguePlayer::RoguePlayer() : m_currentHealth(3), m_maxHealth(3), m_bonusAgility(0), m_bonusDamage(0), m_bonusDefense(0), m_isGhost(false), m_bonusTimer(BONUS_DURATION) {

	const static float32 ANIM_DELAY = 0.07f;

	Texture* tex_walk_s = RenderSystem::get()->getTextureManager()->createTexture2DArray("Rogue_walk_s.tx", Texture::Filter::NEAREST_NEIGHBOR);
	Texture* tex_walk_w = RenderSystem::get()->getTextureManager()->createTexture2DArray("Rogue_walk_w.tx", Texture::Filter::NEAREST_NEIGHBOR);
	Texture* tex_walk_e = RenderSystem::get()->getTextureManager()->createTexture2DArray("Rogue_walk_e.tx", Texture::Filter::NEAREST_NEIGHBOR);
	Texture* tex_walk_n = RenderSystem::get()->getTextureManager()->createTexture2DArray("Rogue_walk_n.tx", Texture::Filter::NEAREST_NEIGHBOR);

	Texture* tex_idle_s = RenderSystem::get()->getTextureManager()->createTexture2D("Rogue_idle_s.tx", Texture::Filter::NEAREST_NEIGHBOR);
	Texture* tex_idle_w = RenderSystem::get()->getTextureManager()->createTexture2D("Rogue_idle_w.tx", Texture::Filter::NEAREST_NEIGHBOR);
	Texture* tex_idle_e = RenderSystem::get()->getTextureManager()->createTexture2D("Rogue_idle_e.tx", Texture::Filter::NEAREST_NEIGHBOR);
	Texture* tex_idle_n = RenderSystem::get()->getTextureManager()->createTexture2D("Rogue_idle_n.tx", Texture::Filter::NEAREST_NEIGHBOR);

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

	CameraComponent* camera = new CameraComponent(this, &RenderSystem::get()->getCamera());
	camera->setSceneBounded(true);
	camera->setSceneBounds(CameraComponent::BOUNDED_ALL);
	addComponent(camera);

	// ################### SWORD #########################
	Texture* swordTexture = RenderSystem::get()->getTextureManager()->createTexture2D("sword.tx", Texture::Filter::NEAREST_NEIGHBOR);
	m_sword = new GameObject();
	m_sword->transform.scale(0.5f);
	SpriteComponent* spComp = new SpriteComponent(m_sword, swordTexture);
	spComp->setVisible(false);
	m_sword->addComponent(spComp);
	SceneManager::get()->getCurrentScene().addGameObject(m_sword);
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
