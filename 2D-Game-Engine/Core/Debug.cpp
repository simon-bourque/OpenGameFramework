#ifdef DEBUG_BUILD

#include "Debug.h"

#include "Console/Console.h"

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

Debug::Debug() : 
	m_zoomIn(false),
	m_zoomOut(false)
{
	getInputInstance()->addKeyListener(KeyListener::create<Debug, &Debug::onKeyPress>(this));
	getInputInstance()->addCursorPositionListener(CursorPositionListener::create<Debug, &Debug::onMouseMove>(this));
	getInputInstance()->addMouseButtonListener(MouseButtonListener::create<Debug, &Debug::onMousePress>(this));
	getInputInstance()->addScrollListener(ScrollListener::create<Debug, &Debug::onMouseScroll>(this));

	Font* font = getRenderSystemInstance()->getFontManager()->createFont("font3");
	m_fpsText = getRenderSystemInstance()->getTextManager()->createText("debug_fps_text_0", "fps: 00", font, Text::Usage::STREAM);

	m_fpsText->setScale(0.5f);

	m_debugFlags["show_fps"] = false;
	m_debugFlags["show_quadtree"] = false;
	m_debugFlags["show_scene_bounds"] = false;
	m_debugFlags["show_grid"] = false;
	m_debugFlags["show_colliders"] = false;
	m_debugFlags["print_key_press"] = false;
	m_debugFlags["print_mouse_press"] = false;
	m_debugFlags["print_mouse_scroll"] = false;
	m_debugFlags["print_mouse_pos"] = false;

	getConsoleInstance()->addCommand("setflag", CommandDelegate::create<Debug, &Debug::setFlagCommand>(this));
	getConsoleInstance()->addCommand("listflag", CommandDelegate::create<Debug, &Debug::listFlagCommand>(this));
}


Debug::~Debug() {}

void Debug::tick(int32 fps) {
	if (m_debugFlags["show_fps"]) {
		m_fpsText->setText("fps: " + std::to_string(fps));
	}
	
	if (m_zoomIn != m_zoomOut) {
		const static float32 SCALE_INCR = 0.02f;
		const static float32 MAX_ZOOM_IN = 0.01f;

		float32 ds = (m_zoomIn) ? -SCALE_INCR : SCALE_INCR;
		float32 scaleX = getRenderSystemInstance()->getCamera().transform.xScale + ds;
		float32 scaleY = getRenderSystemInstance()->getCamera().transform.yScale + ds;

		if (scaleX < MAX_ZOOM_IN) {
			scaleX = MAX_ZOOM_IN;
		}

		if (scaleY < MAX_ZOOM_IN) {
			scaleY = MAX_ZOOM_IN;
		}

		getRenderSystemInstance()->getCamera().transform.xScale = scaleX;
		getRenderSystemInstance()->getCamera().transform.yScale = scaleY;
	}
}

void Debug::render() {
	if (m_debugFlags["show_fps"]) {
		renderPerf();
	}

	if (m_debugFlags["show_grid"]) {
		renderGrid();
	}

	if (m_debugFlags["show_scene_bounds"]) {
		renderBounds();
	}

	if (m_debugFlags["show_quadtree"]) {
		renderQuadTree();
	}
}

void Debug::renderPerf() const {
	getRenderSystemInstance()->getTextRenderer()->renderText(m_fpsText, -0.98f, 0.98f, Color::BLACK);
}

void Debug::renderBounds() const {
	getRenderSystemInstance()->getShapeRenderer()->drawRectangle(getSceneManagerInstance()->getCurrentScene().getBounds(), Color::WHITE, false);
}

void Debug::renderQuadTree() const {
	getSceneManagerInstance()->getCurrentScene().getCollisionSystem()->getQuadTree()->render();
}

void Debug::renderGrid() const {
	geo::Rectangle& bounds = getSceneManagerInstance()->getCurrentScene().getBounds();
	Vector2f vertLine(0,-bounds.getHeight());
	Vector2f horizLine(bounds.getWidth(), 0);
	
	for (int32 i = 0; i < bounds.getWidth() - 1; i++) {
		getRenderSystemInstance()->getShapeRenderer()->drawVector(0.5f + i, 0.5f,vertLine, Color::WHITE);
	}

	for (int32 i = 0; i < bounds.getHeight() - 1; i++) {
		getRenderSystemInstance()->getShapeRenderer()->drawVector(-0.5f, -i - 0.5f, horizLine, Color::WHITE);
	}
}

bool Debug::flag(const string& flag) const {
	auto flagIter = m_debugFlags.find(flag);

	if (flagIter != m_debugFlags.end()) {
		return (*flagIter).second;
	}
	else {
		return false;
	}
}

void Debug::listFlagCommand(const std::vector<string>& args) {
	DEBUG_LOG("---------- FLAGS ----------");
	for (const auto& pair : m_debugFlags) {
		DEBUG_LOG(pair.first);
	}
}

void Debug::setFlagCommand(const std::vector<string>& args) {
	uint32 numArgs = args.size();
	
	if (numArgs > 2 || numArgs < 1) {
		DEBUG_LOG("Error: invalid number of arguments.");
		return;
	}

	auto flagIter = m_debugFlags.find(args[0]);

	if (flagIter != m_debugFlags.end()) {
		if (numArgs == 1) {
			// Toggle flag
			(*flagIter).second = !((*flagIter).second);
		}
		else {
			// Set flag
			int32 i = 0;
			try {
				i = stoi(args[1]);
			}
			catch (std::invalid_argument& ex) {
				DEBUG_LOG("Error: flags can only be set to either 1 or 0.");
				return;
			}
			(*flagIter).second = (i == 0) ? false : true;
		}
	}
	else {
		DEBUG_LOG("Error: flag \'" + args[0] + "\' does not exist.");
	}
}

void Debug::onKeyPress(int32 key, int32 scancode, int32 action, int32 mods) {
	if (m_debugFlags["print_key_press"] && action == Actions::PRESS) {
		string str("KEY: " + std::to_string(key));
		if (mods) {
			str.append("+" + std::to_string(mods));
		}
		DEBUG_LOG(str);
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
	if (key == Keys::KEY_ENTER && action == Actions::PRESS) {
		// Reset zoom
		getRenderSystemInstance()->getCamera().transform.xScale = 1.0f;
		getRenderSystemInstance()->getCamera().transform.yScale = 1.0f;
	}
}

void Debug::onMouseMove(float64 xPos, float64 yPos) {
	if (m_debugFlags["print_mouse_pos"]) {
		string str = std::to_string(xPos) + ", " + std::to_string(yPos);
		DEBUG_LOG(str);
	}
}

void Debug::onMousePress(int32 button, int32 action, int32 mods) {
	if (m_debugFlags["print_mouse_press"]) {
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
}

void Debug::onMouseScroll(float64 xOffset, float64 yOffset) {
	if (m_debugFlags["print_mouse_scroll"]) {
		string str = std::to_string(xOffset) + ", " + std::to_string(yOffset);
		DEBUG_LOG(str);
	}
}

#endif