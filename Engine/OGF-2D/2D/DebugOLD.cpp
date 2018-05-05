#ifdef DEBUG_BUILD

#include "DebugOLD.h"

#include "Core/Console/Console.h"

#include "Core/Graphics/Text/Font.h"
#include "Core/Graphics/Text/FontManager.h"
#include "Core/Graphics/Text/TextManager.h"
#include "Core/Graphics/Text/Text.h"
#include "Core/Graphics/Renderer/TextRenderer.h"
#include "Graphics/Renderer/ShapeRenderer.h"
#include "Core/Graphics/Color.h"
#include "Core/Graphics/GraphicsContext.h"
#include "Graphics/Camera.h"

#include "2D/Graphics/Graphics2D.h"

#include "Core/Window/Window.h"

#include "Physics/Collision/CollisionSystem.h"
#include "Physics/Collision/QuadTree.h"

#include "Scene/SceneManager2D.h"
#include "Scene/Scene2D.h"

#include "Core/Input/Input.h"

#include "Object/Transform.h"

#include "Core/Resource/Resources.h"

#include "Core/Math/Vector2f.h"

DebugOLD::DebugOLD() : 
	m_zoomIn(false),
	m_zoomOut(false)
{
	getInputInstance()->addKeyListener(KeyListener::create<DebugOLD, &DebugOLD::onKeyPress>(this));
	getInputInstance()->addCursorPositionListener(CursorPositionListener::create<DebugOLD, &DebugOLD::onMouseMove>(this));
	getInputInstance()->addMouseButtonListener(MouseButtonListener::create<DebugOLD, &DebugOLD::onMousePress>(this));
	getInputInstance()->addScrollListener(ScrollListener::create<DebugOLD, &DebugOLD::onMouseScroll>(this));

	Font* font = getGraphicsContextInstance()->getFontManager()->createFont("font3");
	m_fpsText = getGraphicsContextInstance()->getTextManager()->createText("debug_fps_text_0", "fps: 00", font, Text::Usage::STREAM);

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

	getConsoleInstance()->addCommand("setflag", CommandDelegate::create<DebugOLD, &DebugOLD::setFlagCommand>(this));
	getConsoleInstance()->addCommand("listflag", CommandDelegate::create<DebugOLD, &DebugOLD::listFlagCommand>(this));
}


DebugOLD::~DebugOLD() {}

void DebugOLD::tick(int32 fps) {
	if (m_debugFlags["show_fps"]) {
		m_fpsText->setText("fps: " + std::to_string(fps));
	}
	
	if (m_zoomIn != m_zoomOut) {
		const static float32 SCALE_INCR = 0.02f;
		const static float32 MAX_ZOOM_IN = 0.01f;

		float32 ds = (m_zoomIn) ? -SCALE_INCR : SCALE_INCR;
		float32 scaleX = getGraphics2DInstance()->getCamera().transform.xScale + ds;
		float32 scaleY = getGraphics2DInstance()->getCamera().transform.yScale + ds;

		if (scaleX < MAX_ZOOM_IN) {
			scaleX = MAX_ZOOM_IN;
		}

		if (scaleY < MAX_ZOOM_IN) {
			scaleY = MAX_ZOOM_IN;
		}

		getGraphics2DInstance()->getCamera().transform.xScale = scaleX;
		getGraphics2DInstance()->getCamera().transform.yScale = scaleY;
	}
}

void DebugOLD::render() {
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

void DebugOLD::renderPerf() const {
	getGraphicsContextInstance()->getTextRenderer()->renderText(m_fpsText, -0.98f, 0.98f, Color::BLACK);
}

void DebugOLD::renderBounds() const {
	getGraphics2DInstance()->getShapeRenderer().drawRectangle(getSceneManager2DInstance()->getCurrentScene().getBounds(), Color::WHITE, false);
}

void DebugOLD::renderQuadTree() const {
	getSceneManager2DInstance()->getCurrentScene().getCollisionSystem()->getQuadTree()->render();
}

void DebugOLD::renderGrid() const {
	geo::Rectangle& bounds = getSceneManager2DInstance()->getCurrentScene().getBounds();
	Vector2f vertLine(0,-bounds.getHeight());
	Vector2f horizLine(bounds.getWidth(), 0);
	
	for (int32 i = 0; i < bounds.getWidth() - 1; i++) {
		getGraphics2DInstance()->getShapeRenderer().drawVector(0.5f + i, 0.5f,vertLine, Color::WHITE);
	}

	for (int32 i = 0; i < bounds.getHeight() - 1; i++) {
		getGraphics2DInstance()->getShapeRenderer().drawVector(-0.5f, -i - 0.5f, horizLine, Color::WHITE);
	}
}

bool DebugOLD::flag(const string& flag) const {
	auto flagIter = m_debugFlags.find(flag);

	if (flagIter != m_debugFlags.end()) {
		return (*flagIter).second;
	}
	else {
		return false;
	}
}

void DebugOLD::listFlagCommand(const std::vector<string>& args) {
	DEBUG_LOG("---------- FLAGS ----------");
	for (const auto& pair : m_debugFlags) {
		DEBUG_LOG(pair.first);
	}
}

void DebugOLD::setFlagCommand(const std::vector<string>& args) {
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

void DebugOLD::onKeyPress(int32 key, int32 scancode, int32 action, int32 mods) {
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
		getGraphics2DInstance()->getCamera().transform.xScale = 1.0f;
		getGraphics2DInstance()->getCamera().transform.yScale = 1.0f;
	}
}

void DebugOLD::onMouseMove(float64 xPos, float64 yPos) {
	if (m_debugFlags["print_mouse_pos"]) {
		string str = std::to_string(xPos) + ", " + std::to_string(yPos);
		DEBUG_LOG(str);
	}
}

void DebugOLD::onMousePress(int32 button, int32 action, int32 mods) {
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

void DebugOLD::onMouseScroll(float64 xOffset, float64 yOffset) {
	if (m_debugFlags["print_mouse_scroll"]) {
		string str = std::to_string(xOffset) + ", " + std::to_string(yOffset);
		DEBUG_LOG(str);
	}
}

#endif