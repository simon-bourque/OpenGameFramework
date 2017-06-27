#include "Player.h"

#include "Core/Core.h"
#include "Core/Game.h"

#include "Object/Component/AnimatorComponent.h"
#include "Object/Component/RigidBodyComponent.h"
#include "Object/Component/AABBColliderComponent.h"
#include "Object/Component/CameraComponent.h"

#include "Graphics/Animation/Animation.h"
#include "Graphics/Animation/AnimState.h"
#include "Graphics/Animation/SpriteAnimState.h"
#include "Graphics/Animation/SpriteSequenceAnimState.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"
#include "Graphics/Window.h"

#include "Input/Input.h"

#include "PlayerController.h"

Player::Player(Game* game, const Vector2f& spawnLocation) {
	const static float32 PLAYER_WIDTH = 1.0f;
	const static float32 PLAYER_HEIGHT = 1.4f;
	const static float32 ANIM_DELAY = 0.05f;

	transform.yScale = PLAYER_HEIGHT;
	transform.translate(spawnLocation);

	// ###################### Animations #####################################
	Texture* walkTexture = RenderSystem::get()->getTextureManager()->createTexture2DArray("player_walk.tx", Texture::Filter::NEAREST_NEIGHBOR);
	Texture* standTexture = RenderSystem::get()->getTextureManager()->createTexture2D("player_stand.tx", Texture::Filter::NEAREST_NEIGHBOR);
	Texture* jumpTexture = RenderSystem::get()->getTextureManager()->createTexture2D("player_jump.tx", Texture::Filter::NEAREST_NEIGHBOR);
	Texture* duckTexture = RenderSystem::get()->getTextureManager()->createTexture2D("player_duck.tx", Texture::Filter::NEAREST_NEIGHBOR);

	uint32 frames[11] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	float32 delays[11] = { ANIM_DELAY, ANIM_DELAY, ANIM_DELAY, ANIM_DELAY, ANIM_DELAY, ANIM_DELAY, ANIM_DELAY, ANIM_DELAY, ANIM_DELAY, ANIM_DELAY, ANIM_DELAY };
	Animation walkAnim(frames, delays, 11);

	AnimState* walkAnimState = new SpriteSequenceAnimState(walkTexture, walkAnim);
	AnimState* standAnimState = new SpriteAnimState(standTexture);
	AnimState* inAirAnimState = new SpriteAnimState(jumpTexture);
	AnimState* duckAnimState = new SpriteAnimState(duckTexture);

	AnimatorComponent* animator = new AnimatorComponent(this, "IDLE", standAnimState);
	animator->addState("WALK", walkAnimState);
	animator->addState("AIR", inAirAnimState);
	animator->addState("DUCK", duckAnimState);

	addComponent(animator);

	// ######################## Physics #####################################
	addComponent(new RigidBodyComponent(this, 65.0f));
	addComponent(new AABBColliderComponent(this, geo::Rectangle(PLAYER_WIDTH, PLAYER_HEIGHT)));

	PlayerController* controller = new PlayerController(this);
	addComponent(controller);

	CameraComponent* camera = new CameraComponent(this, &RenderSystem::get()->getCamera());
	camera->setSceneBounded(true);
	camera->setSceneBounds(CameraComponent::BOUNDED_BOTTOM_BIT | CameraComponent::BOUNDED_LEFT_BIT | CameraComponent::BOUNDED_RIGHT_BIT);
	//camera.setYOffset(2f);
	addComponent(camera);
}

Player::~Player() {}
