#include "PlatformerGame.h"

#include "Core/Core.h"

#include "Graphics/TextureManager.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/Window.h"

#include "Math/Geometry/Rectangle.h"

#include "Input/Input.h"

#include "Scene/SceneManager.h"

#include "Object/GameObject.h"
#include "Object/Component/SpriteComponent.h"
#include "Object/Component/RigidBodyComponent.h"
#include "Object/Component/AnimatorComponent.h"

#include "Graphics/Animation/AnimState.h"
#include "Graphics/Animation/SpriteSequenceAnimState.h"
#include "Graphics/Animation/SpriteAnimState.h"
#include "Graphics/Animation/Animation.h"

PlatformerGame::PlatformerGame() : Game("Platformer", 720, 576, Rectangle(20, 16)), up(false), down(false), left(false), right(false) {}


PlatformerGame::~PlatformerGame() {}

void PlatformerGame::init() {
	getWindow().getInput()->addKeyListener(this, &PlatformerGame::onKeyPress);
	getRenderSystem().getCamera().getTransform().translate(11.0f,-5.5f);
	getSceneManager().loadTileLevel("level_0.lvl", this);

	Texture* gemTexture = getRenderSystem().getTextureManager()->createTexture2D("gemRed.tx", Texture::Filter::NEAREST_NEIGHBOR);

	GameObject* gem1 = new GameObject(Transform(11.0f, -5.5f));
	GameObject* gem2 = new GameObject(Transform(12.0f, -5.5f));
	GameObject* gem3 = new GameObject(Transform(13.0f, -5.5f));
	GameObject* gem4 = new GameObject(Transform(14.0f, -5.5f));

	gem1->addComponent(new SpriteComponent(gem1, gemTexture));
	gem2->addComponent(new SpriteComponent(gem2, gemTexture));
	gem3->addComponent(new SpriteComponent(gem3, gemTexture));
	gem4->addComponent(new SpriteComponent(gem4, gemTexture));

	RigidBodyComponent* rb1 = new RigidBodyComponent(gem1, 60.0f);
	RigidBodyComponent* rb2 = new RigidBodyComponent(gem2, 60.0f);
	RigidBodyComponent* rb3 = new RigidBodyComponent(gem3, 60.0f);
	RigidBodyComponent* rb4 = new RigidBodyComponent(gem4, 60.0f);

	rb1->applyImpulse({ (float32)(((rand() % 2) ? 1:-1) * (rand() % 101 + 100)), (float32)(rand() % 101 + 100)});
	rb2->applyImpulse({ (float32)(((rand() % 2) ? 1 : -1) * (rand() % 101 + 100)), (float32)(rand() % 101 + 100) });
	rb3->applyImpulse({ (float32)(((rand() % 2) ? 1 : -1) * (rand() % 101 + 100)), (float32)(rand() % 101 + 100) });
	rb4->applyImpulse({ (float32)(((rand() % 2) ? 1 : -1) * (rand() % 101 + 100)), (float32)(rand() % 101 + 100) });

	gem1->addComponent(rb1);
	gem2->addComponent(rb2);
	gem3->addComponent(rb3);
	gem4->addComponent(rb4);

	getSceneManager().getCurrentScene().addGameObject(gem1);
	getSceneManager().getCurrentScene().addGameObject(gem2);
	getSceneManager().getCurrentScene().addGameObject(gem3);
	getSceneManager().getCurrentScene().addGameObject(gem4);

	// ##################### ANIM TEST #############################################################
	GameObject* animTestObj = new GameObject(Transform(3.5f, -7.5f));
	
	uint32* frames = new uint32[11]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	float32* delays = new float32[11]{0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f, 0.05f};
	
	Animation animation(frames, delays, 11);

	Texture* standTex = getRenderSystem().getTextureManager()->createTexture2D("player_stand.tx", Texture::Filter::NEAREST_NEIGHBOR);
	Texture* walkTex = getRenderSystem().getTextureManager()->createTexture2DArray("player_walk.tx", Texture::Filter::NEAREST_NEIGHBOR);
	
	AnimState* walkState = new SpriteSequenceAnimState(walkTex, animation);
	AnimState* standState = new SpriteAnimState(standTex);
	animComp = new AnimatorComponent(animTestObj, "STAND", standState);
	animComp->addState("WALK", walkState);
	animTestObj->addComponent(animComp);

	getSceneManager().getCurrentScene().addGameObject(animTestObj);

	delete[] frames;
	delete[] delays;
}

void PlatformerGame::tick(float32 delta) {
	Game::tick(delta);

	const static float32 SPEED = 25.0f;

	if (up) {
		getRenderSystem().getCamera().getTransform().translate(0.0f, SPEED * delta);
	}
	if (down) {
		getRenderSystem().getCamera().getTransform().translate(0.0f, -SPEED * delta);
	}
	if (left) {
		getRenderSystem().getCamera().getTransform().translate(-SPEED * delta, 0.0f);
	}
	if (right) {
		getRenderSystem().getCamera().getTransform().translate(SPEED * delta, 0.0f);
	}
}

void PlatformerGame::render() {
	Game::render();
}

void PlatformerGame::onKeyPress(int32 key, int32 scancode, int32 action, int32 mods) {
	if ((key == Input::KEY_UP || key == Input::KEY_W)) {
		if (action == Input::PRESS) {
			up = true;
		}
		if (action == Input::RELEASE) {
			up = false;
		}
	}
	if ((key == Input::KEY_DOWN || key == Input::KEY_S)) {
		if (action == Input::PRESS) {
			down = true;
		}
		if (action == Input::RELEASE) {
			down = false;
		}
	}
	if ((key == Input::KEY_LEFT || key == Input::KEY_A)) {
		if (action == Input::PRESS) {
			left = true;
		}
		if (action == Input::RELEASE) {
			left = false;
		}
	}
	if ((key == Input::KEY_RIGHT || key == Input::KEY_D)) {
		if (action == Input::PRESS) {
			right = true;
		}
		if (action == Input::RELEASE) {
			right = false;
		}
	}

	if (key == Input::KEY_1 && action == Input::PRESS) {
		animComp->changeState("STAND");
	}

	if (key == Input::KEY_2 && action == Input::PRESS) {
		animComp->changeState("WALK");
	}
}

