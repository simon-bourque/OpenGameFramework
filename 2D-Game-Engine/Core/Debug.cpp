#ifdef DEBUG_BUILD

#include "Debug.h"

#include "Core/Game.h"
#include "Core/EngineAssert.h"

#include "Graphics/Text/Font.h"
#include "Graphics/Text/FontManager.h"
#include "Graphics/Text/TextManager.h"
#include "Graphics/Text/Text.h"
#include "Graphics/Renderer/TextRenderer.h"
#include "Graphics/Renderer/ShapeRenderer.h"
#include "Graphics/Color.h"
#include "Graphics/Window.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/Camera.h"

#include "Physics/Collision/CollisionSystem.h"
#include "Physics/Collision/QuadTree.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

#include "Input/Input.h"

#include "Object/Transform.h"

#include "Resource/Resources.h"

#include "Math/Vector2f.h"

Debug::Debug(Game* game) : 
	m_game(game),
	m_debugMode(false),
	m_renderPerf(false),
	m_renderBounds(false),
	m_renderQuadTree(false),
	m_renderGrid(false),
	m_zoomIn(false),
	m_zoomOut(false)
{
	ASSERT(game, "Game should not be null");
	Input::get()->addKeyListener(this, &Debug::onKeyPress);

	Font* font = RenderSystem::get()->getFontManager()->createFont("font3");
	m_fpsText = RenderSystem::get()->getTextManager()->createText("debug_fps_text_0", "fps: 00", font, Text::Usage::STREAM);
	m_debugOnText = RenderSystem::get()->getTextManager()->createText("debug_debug_on_text_0", "Debug Mode ON", font, Text::Usage::STATIC);
}


Debug::~Debug() {}

void Debug::tick() {
	if (!m_debugMode) {
		return;
	}

	if (m_renderPerf) {
		m_fpsText->setText("fps: " + std::to_string(m_game->getFps()));
	}
	
	if (m_zoomIn != m_zoomOut) {
		const static float32 SCALE_INCR = 0.02f;
		const static float32 MAX_ZOOM_IN = 0.01f;

		float32 ds = (m_zoomIn) ? -SCALE_INCR : SCALE_INCR;
		float32 scaleX = RenderSystem::get()->getCamera().getTransform().xScale + ds;
		float32 scaleY = RenderSystem::get()->getCamera().getTransform().yScale + ds;

		if (scaleX < MAX_ZOOM_IN) {
			scaleX = MAX_ZOOM_IN;
		}

		if (scaleY < MAX_ZOOM_IN) {
			scaleY = MAX_ZOOM_IN;
		}

		RenderSystem::get()->getCamera().getTransform().xScale = scaleX;
		RenderSystem::get()->getCamera().getTransform().yScale = scaleY;
	}
}

void Debug::render() {
	if (!m_debugMode) {
		return;
	}
	RenderSystem::get()->getTextRenderer()->renderText(m_debugOnText, -0.98f, -0.93f, Color::RED);

	if (m_renderPerf) {
		renderPerf();
	}

	if (m_renderGrid) {
		renderGrid();
	}

	if (m_renderBounds) {
		renderBounds();
	}

	if (m_renderQuadTree) {
		renderQuadTree();
	}
}

void Debug::renderPerf() const {
	RenderSystem::get()->getTextRenderer()->renderText(m_fpsText, -0.98f, 0.98f, Color::BLACK);
}

void Debug::renderBounds() const {
	RenderSystem::get()->getShapeRenderer()->drawRectangle(SceneManager::get()->getCurrentScene().getBounds(), Color::WHITE, false);
}

void Debug::renderQuadTree() const {
	SceneManager::get()->getCurrentScene().getCollisionSystem()->getQuadTree()->render();
}

void Debug::renderGrid() const {
	Rectangle& bounds = SceneManager::get()->getCurrentScene().getBounds();
	Vector2f vertLine(0,-bounds.getHeight());
	Vector2f horizLine(bounds.getWidth(), 0);
	
	for (int32 i = 0; i < bounds.getWidth() - 1; i++) {
		RenderSystem::get()->getShapeRenderer()->drawVector(0.5f + i, 0.5f,vertLine, Color::WHITE);
	}

	for (int32 i = 0; i < bounds.getHeight() - 1; i++) {
		RenderSystem::get()->getShapeRenderer()->drawVector(-0.5f, -i - 0.5f, horizLine, Color::WHITE);
	}
}

void Debug::onKeyPress(int32 key, int32 scancode, int32 action, int32 mods) {
	if (key == Input::KEY_GRAVE_ACCENT && (mods & Input::MOD_ALT) && action == Input::PRESS) {
		m_debugMode = !m_debugMode;

		if (m_debugMode) {
			DEBUG_LOG("DEBUG MODE: ON");
		}
		else {
			DEBUG_LOG("DEBUG MODE: OFF");
		}
	}

	if (key == Input::KEY_KP_DECIMAL && action == Input::PRESS && m_debugMode) {
		DEBUG_LOG("---------- SHORTCUTS ----------");
		DEBUG_LOG("KP 0:\t\tToggle performance stats");
		DEBUG_LOG("KP 1:\t\tShow scene bounds");
		DEBUG_LOG("KP 2:\t\tShow quad tree");
		DEBUG_LOG("KP ADD:\t\tZoom in");
		DEBUG_LOG("KP SUBTRACT:\t\tZoom out");
		DEBUG_LOG("KP ENTER:\t\tReset zoom");
	}

	if (key == Input::KEY_KP_0 && m_debugMode) {
		if (action == Input::PRESS) {
			m_renderPerf = !m_renderPerf;
		}
	}

	if (key == Input::KEY_KP_1 && m_debugMode) {
		if (action == Input::PRESS) {
			m_renderBounds = !m_renderBounds;
		}
	}

	if (key == Input::KEY_KP_2 && m_debugMode) {
		if (action == Input::PRESS) {
			m_renderQuadTree = !m_renderQuadTree;
		}
	}

	if (key == Input::KEY_KP_3 && m_debugMode) {
		if (action == Input::PRESS) {
			m_renderGrid = !m_renderGrid;
		}
	}

	if (key == Input::KEY_KP_ADD) {
		if (action == Input::PRESS) {
			m_zoomIn = true;
		}
		if (action == Input::RELEASE) {
			m_zoomIn = false;
		}
	}
	if (key == Input::KEY_KP_SUBTRACT) {
		if (action == Input::PRESS) {
			m_zoomOut = true;
		}
		if (action == Input::RELEASE) {
			m_zoomOut = false;
		}
	}
	if (key == Input::KEY_KP_ENTER && action == Input::PRESS && m_debugMode) {
		// Reset zoom
		RenderSystem::get()->getCamera().getTransform().xScale = 1.0f;
		RenderSystem::get()->getCamera().getTransform().yScale = 1.0f;
	}
}

#endif