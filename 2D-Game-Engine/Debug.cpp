#ifdef DEBUG_BUILD

#include "Debug.h"

#include "Game.h"
#include "Input.h"
#include "Window.h"
#include "RenderSystem.h"
#include "Camera.h"
#include "Transform.h"

Debug::Debug(Game* game) : m_game(game), m_debugMode(false) {
	// TODO add assert here
	game->getWindow().getInput()->addKeyListener(this, &Debug::onKeyPress);
}


Debug::~Debug() {}

void Debug::tick() {
	if (!m_debugMode) {
		return;
	}
	
	if (m_zoomIn != m_zoomOut) {
		const static float32 SCALE_INCR = 0.02f;
		const static float32 MAX_ZOOM_IN = 0.01f;

		float32 ds = (m_zoomIn) ? -SCALE_INCR : SCALE_INCR;
		float32 scaleX = m_game->getRenderSystem().getCamera().getTransform().xScale + ds;
		float32 scaleY = m_game->getRenderSystem().getCamera().getTransform().yScale + ds;

		if (scaleX < MAX_ZOOM_IN) {
			scaleX = MAX_ZOOM_IN;
		}

		if (scaleY < MAX_ZOOM_IN) {
			scaleY = MAX_ZOOM_IN;
		}

		m_game->getRenderSystem().getCamera().getTransform().xScale = scaleX;
		m_game->getRenderSystem().getCamera().getTransform().yScale = scaleY;
	}
}

void Debug::render() {
	if (!m_debugMode) {
		return;
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
		DEBUG_LOG("KP ADD:\t\tZoom in");
		DEBUG_LOG("KP SUBTRACT:\t\tZoom out");
		DEBUG_LOG("KP ENTER:\t\tReset zoom");
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
		m_game->getRenderSystem().getCamera().getTransform().xScale = 1.0f;
		m_game->getRenderSystem().getCamera().getTransform().yScale = 1.0f;
	}
}

#endif