#include "PlayerController.h"

#include "Core/Event.h"

#include "Object/GameObject.h"
#include "Object/Component/ComponentType.h"
#include "Input/Input.h"

PlayerController::PlayerController(GameObject* parentObject) : 
	ObjectComponent(parentObject),
	m_lastDirection(NONE),
	m_upAction(false),
	m_downAction(false),
	m_leftAction(false),
	m_rightAction(false)
{
	Input::get()->addKeyListener(this, &PlayerController::onKey);
}

PlayerController::~PlayerController() {}

void PlayerController::tick(float32 delta) {
	const static float32 SPEED = 5.0f;

	Direction direction = getDirection();

	string str("IDLE_S");
	switch (direction) {
	case NORTH:
		if (direction != m_lastDirection) {
			str = "WALK_N";
			getParentObject()->broadcastEvent(Event(Event::Type::ANIM_STATE_CHANGE, &str));
		}
		getParentObject()->getTransform().translate(0, SPEED * delta);
		break;
	case SOUTH:
		if (direction != m_lastDirection) {
			str = "WALK_S";
			getParentObject()->broadcastEvent(Event(Event::Type::ANIM_STATE_CHANGE, &str));
		}
		getParentObject()->getTransform().translate(0, -SPEED * delta);
		break;
	case WEST:
		if (direction != m_lastDirection) {
			str = "WALK_W";
			getParentObject()->broadcastEvent(Event(Event::Type::ANIM_STATE_CHANGE, &str));
		}
		getParentObject()->getTransform().translate(-SPEED * delta, 0);
		break;
	case EAST:
		if (direction != m_lastDirection) {
			str = "WALK_E";
			getParentObject()->broadcastEvent(Event(Event::Type::ANIM_STATE_CHANGE, &str));
		}
		getParentObject()->getTransform().translate(SPEED * delta, 0);
		break;
	case NONE:
		if (direction != m_lastDirection) {
			if (m_lastDirection == NORTH) {
				str = "IDLE_N";
			}
			else if (m_lastDirection == SOUTH) {
				str = "IDLE_S";
			}
			else if (m_lastDirection == EAST) {
				str = "IDLE_E";
			}
			else if (m_lastDirection == WEST) {
				str = "IDLE_W";
			}
			getParentObject()->broadcastEvent(Event(Event::Type::ANIM_STATE_CHANGE, &str));
		}
	}

	if (m_upAction) {
		//getParentObject()->getTransform().translate(0, SPEED * delta);
	}
	if (m_downAction) {
		//getParentObject()->getTransform().translate(0, -SPEED * delta);
	}
	if (m_leftAction) {
		//getParentObject()->getTransform().translate(-SPEED * delta, 0);
	}
	if (m_rightAction) {
		//getParentObject()->getTransform().translate(SPEED * delta, 0);
	}
	m_lastDirection = direction;
}

PlayerController::Direction PlayerController::getDirection() const {
	if (m_upAction && !m_downAction && !m_rightAction && !m_leftAction) {
		return NORTH;
	}
	if (!m_upAction && m_downAction && !m_rightAction && !m_leftAction) {
		return SOUTH;
	}
	if (!m_upAction && !m_downAction && m_rightAction && !m_leftAction) {
		return EAST;
	}
	if (!m_upAction && !m_downAction && !m_rightAction && m_leftAction) {
		return WEST;
	}

	return NONE;
}

ComponentType PlayerController::getType() {
	return ComponentType::DEFAULT;
}

void PlayerController::receiveEvent(const Event& event) {

}

void PlayerController::onKey(int32 key, int32 scancode, int32 action, int32 mods) {
	if (key == Input::KEY_W || key == Input::KEY_UP) {
		if (action == Input::PRESS) {
			m_upAction = true;
		}
		if (action == Input::RELEASE) {
			m_upAction = false;
		}
	}
	
	if (key == Input::KEY_A || key == Input::KEY_LEFT) {
		if (action == Input::PRESS) {
			m_leftAction = true;
		}
		if (action == Input::RELEASE) {
			m_leftAction = false;
		}
	}
	
	if (key == Input::KEY_S || key == Input::KEY_DOWN) {
		if (action == Input::PRESS) {
			m_downAction = true;
		}
		if (action == Input::RELEASE) {
			m_downAction = false;
		}
	}
	
	if (key == Input::KEY_D || key == Input::KEY_RIGHT) {
		if (action == Input::PRESS) {
			m_rightAction = true;
		}
		if (action == Input::RELEASE) {
			m_rightAction = false;
		}
	}
}

#ifdef DEBUG_BUILD
void PlayerController::debugRender() {}
#endif