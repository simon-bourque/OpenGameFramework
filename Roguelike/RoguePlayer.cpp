#include "RoguePlayer.h"

#include "PlayerController.h"

#include "Object/Component/CameraComponent.h"
#include "Object/Component/AABBColliderComponent.h"
#include "Object/Component/AnimatorComponent.h"

#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/Animation/Animation.h"
#include "Graphics/Animation/SpriteSequenceAnimState.h"
#include "Graphics/Animation/SpriteAnimState.h"

RoguePlayer::RoguePlayer() {

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
	
	addComponent(new AABBColliderComponent(this,Rectangle(1,1)));
	addComponent(new PlayerController(this));

	CameraComponent* camera = new CameraComponent(this, &RenderSystem::get()->getCamera());
	camera->setSceneBounded(true);
	camera->setSceneBounds(CameraComponent::BOUNDED_ALL);
	addComponent(camera);
}

RoguePlayer::~RoguePlayer() {
}
