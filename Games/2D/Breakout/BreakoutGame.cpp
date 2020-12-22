#include "BreakoutGame.h"

#include "GameObjects/Paddle.h"
#include "GameObjects/Ball.h"
#include "GameObjects/Level.h"

#include "Core/Input/Input.h"
#include "Core/Math/Geometry/Rectangle.h"
#include "Core/Graphics/GraphicsContext.h"
#include "Core/Graphics/TextureCache.h"
#include "Core/Graphics/Text/Font.h"
#include "Core/Graphics/Text/FontManager.h"
#include "Core/Graphics/Text/TextManager.h"
#include "Core/Graphics/Text/Text.h"
#include "Core/Graphics/Color.h"
#include "Core/Graphics/Renderer/TextRenderer.h"

#include "2D/Scene/Background.h"
#include "2D/Scene/SceneManager2D.h"

#include <stdlib.h>
#include <time.h>

BreakoutGame::BreakoutGame()
	: Game2D("Breakout", 933, 700, geo::Rectangle(40, 30)) // Rectangle is number of game units on screen
{
}

void BreakoutGame::init()
{
	// Initialize random seed
	srand(time(NULL));

	getInputInstance()->addKeyListener(KeyListener::create<BreakoutGame, &BreakoutGame::onKeyPress>(this));

	TextureRef bgTextureRef = getGraphicsContextInstance()->getTextureCache()->loadTexture("background.tx");
	getSceneManager2DInstance()->getCurrentScene().addBackground(new Background(bgTextureRef));

	m_level = new Level(Level::Template::LEVEL1);
	getSceneManager2DInstance()->getCurrentScene().addGameObject(m_level);

	m_ball = new Ball();
	getSceneManager2DInstance()->getCurrentScene().addGameObject(m_ball);

	m_paddle = new Paddle();
	getSceneManager2DInstance()->getCurrentScene().addGameObject(m_paddle);

	m_font = getGraphicsContextInstance()->getFontManager()->createFont("font3");
	m_startText = getGraphicsContextInstance()->getTextManager()->createText("start_text", "Press SPACE to undock ball", m_font, Text::Usage::STREAM);
	m_optionsText = getGraphicsContextInstance()->getTextManager()->createText("start_text", "Press R to dock ball, Press F to reset level", m_font, Text::Usage::STREAM);
	m_optionsText->setScale(0.5f);
}

void BreakoutGame::render()
{
	Game2D::render();
	if (m_ball->isDocked())
	{
		getGraphicsContextInstance()->getTextRenderer()->renderText(m_startText, -0.5f, 0.0f, Color::WHITE);
		getGraphicsContextInstance()->getTextRenderer()->renderText(m_optionsText, -0.45f, -0.1f, Color::WHITE);
	}
}

void BreakoutGame::tick(float32 delta)
{
	if (m_ball->isOutOfBounds())
	{
		m_ball->setDocked(true);
		m_level->resetLevel();
	}

	if (m_ball->isDocked())
	{
		Vector2f paddlePos = m_paddle->getPosition();
		m_ball->transform.xPos = paddlePos.x;
		m_ball->transform.yPos = paddlePos.y + Paddle::PADDLE_HEIGHT;
	}

	if (m_level->attemptPowerupConsume())
	{
		switch (m_level->getPowerupType())
		{
		case Powerup::PowerType::XBALL:
			m_ball->activateXBallMode();
			break;
		case Powerup::PowerType::REBUILD:
			m_ball->setDocked(true);
			m_level->resetLevel();
			break;
		}
	}

	Game2D::tick(delta);
}

void BreakoutGame::onKeyPress(int32 key, int32 scancode, int32 action, int32 mods)
{
	if (key == Keys::KEY_ESCAPE && action == Actions::PRESS)
	{
		shutdown();
	}
	if (key == Keys::KEY_SPACE && action == Actions::PRESS)
	{
		if (m_ball)
		{
			m_ball->setDocked(false);
		}
	}
	if (key == Keys::KEY_R && action == Actions::PRESS)
	{
		if (m_ball)
		{
			m_ball->setDocked(true);
		}
	}
	if (key == Keys::KEY_F && action == Actions::PRESS)
	{
		if (m_level)
		{
			m_level->resetLevel();
		}
	}
}

BreakoutGame::~BreakoutGame()
{
}
