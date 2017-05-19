#include "RoguelikeGame.h"

#include "Math/Geometry/Rectangle.h"
#include "Scene/SceneManager.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/Camera.h"
#include "Object/Transform.h"

RoguelikeGame::RoguelikeGame() : Game("Roguelike", 720, 576, Rectangle(20,20)) {}


RoguelikeGame::~RoguelikeGame() {}

void RoguelikeGame::init() {
	SceneManager::get()->loadTileLevel("level_test.lvl");
	RenderSystem::get()->getCamera().getTransform().translate(47,-53);
}

void RoguelikeGame::render() {
	Game::render();
}

void RoguelikeGame::tick(float32 delta) {
	Game::tick(delta);
}
