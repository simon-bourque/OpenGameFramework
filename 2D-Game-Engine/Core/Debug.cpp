#ifdef DEBUG_BUILD

#include "Debug.h"

#include "Core/Game.h"

#include "Graphics/Text/Font.h"
#include "Graphics/Text/FontManager.h"
#include "Graphics/Text/TextManager.h"
#include "Graphics/Text/Text.h"
#include "Graphics/Renderer/TextRenderer.h"
#include "Graphics/Renderer/ShapeRenderer.h"
#include "Graphics/Color.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/Camera.h"

#include "Window/Window.h"

#include "Physics/Collision/CollisionSystem.h"
#include "Physics/Collision/QuadTree.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

#include "Input/Input.h"

#include "Object/Transform.h"

#include "Resource/Resources.h"

#include "Math/Vector2f.h"

Debug* Debug::s_instance = nullptr;

Debug::Debug() : 
	m_debugMode(false),
	m_renderColliders(false),
	m_renderPerf(false),
	m_renderBounds(false),
	m_renderQuadTree(false),
	m_renderGrid(false),
	m_zoomIn(false),
	m_zoomOut(false)
{
	Input::get()->addKeyListener(this, &Debug::onKeyPress);
	Input::get()->addCursorPositionListener(this, &Debug::onMouseMove);
	Input::get()->addMouseButtonListener(this, &Debug::onMousePress);
	Input::get()->addScrollListener(this, &Debug::onMouseScroll);

	Font* font = RenderSystem::get()->getFontManager()->createFont("font3");
	m_fpsText = RenderSystem::get()->getTextManager()->createText("debug_fps_text_0", "fps: 00", font, Text::Usage::STREAM);
	m_debugOnText = RenderSystem::get()->getTextManager()->createText("debug_debug_on_text_0", "Debug Mode ON", font, Text::Usage::STATIC);

	m_fpsText->setScale(0.5f);
	m_debugOnText->setScale(0.5f);
}


Debug::~Debug() {}

void Debug::tick(int32 fps) {
	if (!m_debugMode) {
		return;
	}

	if (m_renderPerf) {
		m_fpsText->setText("fps: " + std::to_string(fps));
	}
	
	if (m_zoomIn != m_zoomOut) {
		const static float32 SCALE_INCR = 0.02f;
		const static float32 MAX_ZOOM_IN = 0.01f;

		float32 ds = (m_zoomIn) ? -SCALE_INCR : SCALE_INCR;
		float32 scaleX = RenderSystem::get()->getCamera().transform.xScale + ds;
		float32 scaleY = RenderSystem::get()->getCamera().transform.yScale + ds;

		if (scaleX < MAX_ZOOM_IN) {
			scaleX = MAX_ZOOM_IN;
		}

		if (scaleY < MAX_ZOOM_IN) {
			scaleY = MAX_ZOOM_IN;
		}

		RenderSystem::get()->getCamera().transform.xScale = scaleX;
		RenderSystem::get()->getCamera().transform.yScale = scaleY;
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
	geo::Rectangle& bounds = SceneManager::get()->getCurrentScene().getBounds();
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
	//if (action == Actions::PRESS) {
	//	if (mods) {
	//		DEBUG_LOG(std::to_string(mods) + "+");
	//	}
	//	DEBUG_LOG(std::to_string(key));
	//}

	if (key == Keys::KEY_GRAVE_ACCENT && (mods & Mods::MOD_ALT_BIT) && action == Actions::PRESS) {
		m_debugMode = !m_debugMode;

		if (m_debugMode) {
			DEBUG_LOG("DEBUG MODE: ON");
		}
		else {
			DEBUG_LOG("DEBUG MODE: OFF");
		}
	}

	if (key == Keys::KEY_KP_DECIMAL && action == Actions::PRESS && m_debugMode) {
		DEBUG_LOG("---------- SHORTCUTS ----------");
		DEBUG_LOG("KP 0:\t\tToggle performance stats");
		DEBUG_LOG("KP 1:\t\tShow scene bounds");
		DEBUG_LOG("KP 2:\t\tShow quad tree");
		DEBUG_LOG("KP ADD:\t\tZoom in");
		DEBUG_LOG("KP SUBTRACT:\t\tZoom out");
		DEBUG_LOG("KP ENTER:\t\tReset zoom");
	}

	if (key == Keys::KEY_KP_0 && m_debugMode) {
		if (action == Actions::PRESS) {
			m_renderPerf = !m_renderPerf;
		}
	}

	if (key == Keys::KEY_KP_1 && m_debugMode) {
		if (action == Actions::PRESS) {
			m_renderBounds = !m_renderBounds;
		}
	}

	if (key == Keys::KEY_KP_2 && m_debugMode) {
		if (action == Actions::PRESS) {
			m_renderQuadTree = !m_renderQuadTree;
		}
	}

	if (key == Keys::KEY_KP_3 && m_debugMode) {
		if (action == Actions::PRESS) {
			m_renderGrid = !m_renderGrid;
		}
	}

	if (key == Keys::KEY_KP_4 && m_debugMode) {
		if (action == Actions::PRESS) {
			m_renderColliders = !m_renderColliders;
		}
	}

	if (key == Keys::KEY_KP_ADD) {
		if (action == Actions::PRESS) {
			m_zoomIn = true;
		}
		if (action == Actions::RELEASE) {
			m_zoomIn = false;
		}
	}
	if (key == Keys::KEY_KP_SUBTRACT) {
		if (action == Actions::PRESS) {
			m_zoomOut = true;
		}
		if (action == Actions::RELEASE) {
			m_zoomOut = false;
		}
	}
	if (key == Keys::KEY_ENTER && action == Actions::PRESS && m_debugMode) {
		// Reset zoom
		RenderSystem::get()->getCamera().transform.xScale = 1.0f;
		RenderSystem::get()->getCamera().transform.yScale = 1.0f;
	}
}

void Debug::onMouseMove(float64 xPos, float64 yPos) {
	//string str = std::to_string(xPos) + ", " + std::to_string(yPos);
	//DEBUG_LOG(str);
}

void Debug::onMousePress(int32 button, int32 action, int32 mods) {
	if (button == MouseButtons::MOUSE_BUTTON_LEFT && action == Actions::PRESS) {
		DEBUG_LOG("LEFT CLICK");
	}
	if (button == MouseButtons::MOUSE_BUTTON_RIGHT && action == Actions::PRESS) {
		DEBUG_LOG("RIGHT CLICK");
	}
	if (button == MouseButtons::MOUSE_BUTTON_MIDDLE && action == Actions::PRESS) {
		DEBUG_LOG("MIDDLE CLICK");
	}
	if (button == MouseButtons::MOUSE_BUTTON_4 && action == Actions::PRESS) {
		DEBUG_LOG("X1 CLICK");
	}
	if (button == MouseButtons::MOUSE_BUTTON_5 && action == Actions::PRESS) {
		DEBUG_LOG("X2 CLICK");
	}
}

void Debug::onMouseScroll(float64 xOffset, float64 yOffset) {
	string str = std::to_string(xOffset) + ", " + std::to_string(yOffset);
	DEBUG_LOG(str);
}

#endif