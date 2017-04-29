#ifdef DEBUG_BUILD

#include "Debug.h"

#include "Game.h"
#include "Input.h"
#include "Window.h"

Debug::Debug(Game* game) : m_game(game), m_debugMode(false) {
	// TODO add assert here
	game->getWindow().getInput()->addKeyListener(this, &Debug::onKeyPress);
}


Debug::~Debug() {}

void Debug::render() {

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
		DEBUG_LOG("KP 0: TODO");
		DEBUG_LOG("KP 1: TODO");
	}
}

#endif