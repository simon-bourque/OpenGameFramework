#include "PlatformerGame.h"

#include "Rectangle.h"
#include "Resources.h"
#include "RawImage.h"
#include "RenderSystem.h"
#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "Window.h"
#include "Input.h"
#include "TileScene.h"
#include "Resources.h"
#include "SceneRenderer.h"
#include "SceneManager.h"

#include "Core.h"

#include <iostream>

PlatformerGame::PlatformerGame() : Game("Platformer", 720, 576, Rectangle(20, 16)), up(false), down(false), left(false), right(false) {}


PlatformerGame::~PlatformerGame() {}

void PlatformerGame::init() {
	getWindow().getInput()->addKeyListener(this, &PlatformerGame::onKeyPress);
	getRenderSystem().getCamera().getTransform().translate(11.0f,-5.5f);
	getSceneManager().loadTileLevel("level_0.lvl", this);
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
}

