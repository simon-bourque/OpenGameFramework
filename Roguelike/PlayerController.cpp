#include "PlayerController.h"

#include "Core/Event.h"

#include "Object/GameObject.h"
#include "Object/Component/ComponentType.h"
#include "Object/Component/SpriteComponent.h"
#include "Input/Input.h"

#include "Sound/SoundEngine.h"

#include "RoguePlayer.h"

PlayerController::PlayerController(GameObject* parentObject, RoguePlayer* player) :
	ObjectComponent(parentObject),
	m_lastDirection(NONE),
	m_upAction(false),
	m_downAction(false),
	m_leftAction(false),
	m_rightAction(false),
	m_player(player),
	m_swordOffset(0.4f),
	m_swordCountdown(1.0f),
	m_walkDelay(0.0f),
	m_swingingSword(false)
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
			SpriteComponent* cp = (SpriteComponent*)(m_player->getSword()->findComponent(ComponentType::SPRITE_COMPONENT));
			cp->setHorizontalFlip(false);
			m_swordOffset = -0.4f;
		}
		getParentObject()->getTransform().translate(-SPEED * delta, 0);
		break;
	case EAST:
		if (direction != m_lastDirection) {
			str = "WALK_E";
			getParentObject()->broadcastEvent(Event(Event::Type::ANIM_STATE_CHANGE, &str));
			SpriteComponent* cp = (SpriteComponent*)(m_player->getSword()->findComponent(ComponentType::SPRITE_COMPONENT));
			cp->setHorizontalFlip(true);
			m_swordOffset = 0.4f;
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

	if (direction != NONE) {
		m_walkDelay += delta;
		if (m_walkDelay > 0.1f) {
			m_walkDelay = 0.0f;
			SoundEngine::get()->playSound("res/sound/walk.wav"); 
		}
	}

	m_lastDirection = direction;

	// Move sword
		m_player->getSword()->getTransform().xPos = getParentObject()->getTransform().xPos + m_swordOffset;
		m_player->getSword()->getTransform().yPos = getParentObject()->getTransform().yPos;

	if (m_swingingSword) {
		m_swordCountdown -= delta;

		if (m_swordCountdown <= 0) {
			SpriteComponent* cp = (SpriteComponent*)(m_player->getSword()->findComponent(ComponentType::SPRITE_COMPONENT));
			cp->setVisible(false);
			m_swingingSword = false;
		}
	}
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

	if (key == Input::KEY_SPACE) {
		if (action == Input::PRESS && !m_swingingSword) {
			SpriteComponent* cp = (SpriteComponent*)(m_player->getSword()->findComponent(ComponentType::SPRITE_COMPONENT));
			m_swordCountdown = 0.1f;
			cp->setVisible(true);
			m_swingingSword = true;
			SoundEngine::get()->playSound("res/sound/swish.wav");
		}
	}
}

#ifdef DEBUG_BUILD
void PlayerController::debugRender() {}
#endif