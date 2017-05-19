#include "RoguelikeGame.h"

#include "Math/Geometry/Rectangle.h"

RoguelikeGame::RoguelikeGame() : Game("Roguelike", 720, 576, Rectangle(20,20)) {}


RoguelikeGame::~RoguelikeGame() {}

void RoguelikeGame::init() {

}

void RoguelikeGame::render() {
	Game::render();
}

void RoguelikeGame::tick(float32 delta) {
	Game::tick(delta);
}
