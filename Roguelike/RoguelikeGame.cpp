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
#include "Graphics/TextureManager.h"
#include "Graphics/Texture.h"

#include "RoguePlayer.h"

#include "Input/Input.h"

#include "UI/UserInterface.h"
#include "UI/UIComponent.h"
#include "HealthBar.h"
#include "InventoryUI.h"

RoguelikeGame::RoguelikeGame() : Game("Roguelike", 720, 576, geo::Rectangle(20,20)) {}


RoguelikeGame::~RoguelikeGame() {
	delete m_ui;
}

void RoguelikeGame::init() {
	Input::get()->addKeyListener(KeyListener::create<RoguelikeGame, &RoguelikeGame::onKey>(this));

	SceneManager::get()->loadTileLevel("level_test2.lvl");

	m_player = new RoguePlayer();
	m_player->transform.translate(6.5f, -89);
	SceneManager::get()->getCurrentScene().addGameObject(m_player);

	m_mainFont = RenderSystem::get()->getFontManager()->createFont("const");
	m_testText = RenderSystem::get()->getTextManager()->createText("test_text_69", "abcdefghijklmnopqrstuvwxyz", m_mainFont, Text::Usage::STATIC);

	m_ui = new UserInterface();
	m_invUI = new InventoryUI();
	m_invUI->setVisible(false);
	m_ui->addUIComponent(new HealthBar(m_player));
	m_ui->addUIComponent(m_invUI);

	//SoundEngine::get()->playMusic("res/sound/wily.ogg", true, musicType::FOREGROUND);
}

void RoguelikeGame::render() {
	Game::render();
	RenderSystem::get()->getTextRenderer()->renderText(m_testText, -0.9,0.9, Color::WHITE);
	m_ui->render();
}

void RoguelikeGame::tick(float32 delta) {
	Game::tick(delta);
}

void RoguelikeGame::onKey(int32 key, int32 scancode, int32 action, int32 mods) {
	if (key == Keys::KEY_MINUS && action == Actions::PRESS) {
		m_player->setCurrentHealth(m_player->getCurrentHealth() - 1);
	}
	if (key == Keys::KEY_I && action == Actions::PRESS) {
		// Toggle inventory
		m_invUI->setVisible(!m_invUI->isVisible());
		//SoundEngine::get()->playSound("res/sound/inventory.wav");
		SceneManager::get()->setPaused(!SceneManager::get()->isPaused());
	}
}