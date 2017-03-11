#include "PlatformerGame.h"

#include "Rectangle.h"
#include "Resources.h"
#include "RawImage.h"
#include "RenderSystem.h"
#include "SpriteRenderer.h"

PlatformerGame::PlatformerGame() : Game("Platformer", 720, 576, Rectangle(20, 16)) {}


PlatformerGame::~PlatformerGame() {
	delete testTexture;
}

void PlatformerGame::init() {
	testTexture = Texture::createTexture2D(loadImage("../res/texture/boxItem.png"), Texture::Filter::NEAREST_NEIGHBOR, Texture::Wrap::CLAMP_TO_EDGE, Texture::Wrap::CLAMP_TO_EDGE);
}

void PlatformerGame::render() {
	Game::render();

	getRenderSystem()->getSpriteRenderer()->renderSprite(&testT, testTexture, false, false);
}

