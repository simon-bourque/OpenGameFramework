#include "PlatformerGame.h"

#include "Rectangle.h"
#include "Resources.h"
#include "RawImage.h"
#include "RenderSystem.h"
#include "SpriteRenderer.h"
#include "TextureManager.h"
#include "Window.h"
#include "Input.h"

#include "Core.h"

#include <iostream>

PlatformerGame::PlatformerGame() : Game("Platformer", 720, 576, Rectangle(20, 16)), up(false), down(false), left(false), right(false) {}


PlatformerGame::~PlatformerGame() {

}

void PlatformerGame::init() {
	testTexture = getRenderSystem()->getTextureManager()->createTexture2D(loadImage("../res/texture/boxItem.png"), Texture::Filter::NEAREST_NEIGHBOR, Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap::CLAMP_TO_EDGE);
	getWindow()->getInput()->addKeyListener(this, &PlatformerGame::onKeyPress);
	getWindow()->getInput()->addCursorPositionListener(this, &PlatformerGame::onMouseMove);
}

void PlatformerGame::tick(float32 delta) {
	Game::tick(delta);

	if (up) {
		getRenderSystem()->getCamera().getTransform().translate(0.0f, -0.1f);
	}
	if (down) {
		getRenderSystem()->getCamera().getTransform().translate(0.0f, 0.1f);
	}
	if (left) {
		getRenderSystem()->getCamera().getTransform().translate(0.1f, 0.0f);
	}
	if (right) {
		getRenderSystem()->getCamera().getTransform().translate(-0.1f, 0.0f);
	}
}

void PlatformerGame::render() {
	Game::render();

	getRenderSystem()->getSpriteRenderer()->renderSprite(&testT, testTexture, false, false);
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

void PlatformerGame::onMouseMove(float64 xPos, float64 yPos) {
	cout << "<" << xPos << ", " << yPos << ">" << endl;
}

