#include "RoguelikeGame.h"

#include "Math/Geometry/Rectangle.h"
#include "Scene/SceneManager.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/Camera.h"
#include "Object/Transform.h"

#include "Graphics/Text/Font.h"
#include "Graphics/Text/FontManager.h"
#include "Graphics/Text/TextManager.h"
#include "Graphics/Text/Text.h"
#include "Graphics/Renderer/TextRenderer.h"
#include "Graphics/Color.h"

#include "RoguePlayer.h"

RoguelikeGame::RoguelikeGame() : Game("Roguelike", 720, 576, Rectangle(20,20)) {}


RoguelikeGame::~RoguelikeGame() {}

void RoguelikeGame::init() {
	SceneManager::get()->loadTileLevel("level_test.lvl");
	RenderSystem::get()->getCamera().getTransform().translate(54,-53);

	RoguePlayer* player = new RoguePlayer();
	player->getTransform().translate(54, -53);
	SceneManager::get()->getCurrentScene().addGameObject(player);

	m_mainFont = RenderSystem::get()->getFontManager()->createFont("const");
	m_testText = RenderSystem::get()->getTextManager()->createText("test_text_69", "abcdefghijklmnopqrstuvwxyz", m_mainFont, Text::Usage::STATIC);
}

void RoguelikeGame::render() {
	Game::render();
	RenderSystem::get()->getTextRenderer()->renderText(m_testText, -0.9,0.9, Color::WHITE);
}

void RoguelikeGame::tick(float32 delta) {
	Game::tick(delta);
}
