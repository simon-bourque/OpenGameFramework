#include "PlayerController.h"

#include "Core/Event.h"

#include "2D/Object/GameObject.h"
#include "2D/Object/Component/ComponentType.h"
#include "2D/Object/Component/SpriteComponent.h"
#include "Input/Input.h"

#include "Math/Vector2f.h"
#include "Math/Constants.h"

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
	getInputInstance()->addKeyListener(KeyListener::create<PlayerController, &PlayerController::onKey>(this));
}

PlayerController::~PlayerController() {}

void PlayerController::tick(float32 delta) {
	const static float32 SPEED = 5.0f;

	Direction direction = getDirection();

	movePlayer(direction, delta);
	selectAnimation(direction);
	flipSwordSprite(direction);

	if (direction != NONE) {
		m_walkDelay += delta;
		if (m_walkDelay > 0.1f) {
			m_walkDelay = 0.0f;
			//SoundEngine::get()->playSound("res/sound/walk.wav"); 
		}
	}

	m_lastDirection = direction;

	// Move sword
	m_player->getSword()->transform.xPos = getParentObject()->transform.xPos + m_swordOffset;
	m_player->getSword()->transform.yPos = getParentObject()->transform.yPos;

	if (m_swingingSword) {
		m_swordCountdown -= delta;

		if (m_swordCountdown <= 0) {
			SpriteComponent* cp = m_player->getSword()->findComponent<SpriteComponent>();
			cp->setVisible(false);
			m_swingingSword = false;
		}
	}
}

PlayerController::Direction PlayerController::getDirection() const {
	if ((m_upAction && !m_downAction && !m_rightAction && !m_leftAction) || (m_upAction && !m_downAction && m_rightAction && m_leftAction)) {
		return NORTH;
	}
	if (m_upAction && !m_downAction && !m_rightAction && m_leftAction) {
		return NORTH_WEST;
	}
	if (m_upAction && !m_downAction && m_rightAction && !m_leftAction) {
		return NORTH_EAST;
	}
	if ((!m_upAction && m_downAction && !m_rightAction && !m_leftAction) || (!m_upAction && m_downAction && m_rightAction && m_leftAction)) {
		return SOUTH;
	}
	if (!m_upAction && m_downAction && !m_rightAction && m_leftAction) {
		return SOUTH_WEST;
	}
	if (!m_upAction && m_downAction && m_rightAction && !m_leftAction) {
		return SOUTH_EAST;
	}
	if ((!m_upAction && !m_downAction && m_rightAction && !m_leftAction) || (m_upAction && m_downAction && m_rightAction && !m_leftAction)) {
		return EAST;
	}
	if ((!m_upAction && !m_downAction && !m_rightAction && m_leftAction) || (m_upAction && m_downAction && !m_rightAction && m_leftAction)) {
		return WEST;
	}

	return NONE;
}

void PlayerController::flipSwordSprite(Direction direction) {
	if (direction == m_lastDirection) {
		return;
	}

	if (direction == EAST || direction == NORTH_EAST || direction == SOUTH_EAST) {
		SpriteComponent* cp = m_player->getSword()->findComponent<SpriteComponent>();
		cp->setHorizontalFlip(true);
		m_swordOffset = 0.4f;
	}
	else if (direction == WEST || direction == NORTH_WEST || direction == SOUTH_WEST) {
		SpriteComponent* cp = m_player->getSword()->findComponent<SpriteComponent>();
		cp->setHorizontalFlip(false);
		m_swordOffset = -0.4f;
	}
}

void PlayerController::selectAnimation(Direction direction) {
	if (direction == m_lastDirection) {
		return;
	}

	string str("IDLE_S");
	if (direction == NORTH) {
		str = "WALK_N";
	}
	else if (direction == SOUTH) {
		str = "WALK_S";
	}
	else if (direction == NORTH_WEST || direction == SOUTH_WEST || direction == WEST) {
		str = "WALK_W";
	}
	else if (direction == NORTH_EAST || direction == SOUTH_EAST || direction == EAST) {
		str = "WALK_E";
	}
	else {
		if (m_lastDirection == NORTH) {
			str = "IDLE_N";
		}
		else if (m_lastDirection == SOUTH) {
			str = "IDLE_S";
		}
		else if (m_lastDirection == EAST || m_lastDirection == NORTH_EAST || m_lastDirection == SOUTH_EAST) {
			str = "IDLE_E";
		}
		else if (m_lastDirection == WEST || m_lastDirection == NORTH_WEST || m_lastDirection == SOUTH_WEST) {
			str = "IDLE_W";
		}
	}

	getParentObject()->broadcastEvent(Event(Event::Type::ANIM_STATE_CHANGE, &str));
}

void PlayerController::movePlayer(Direction direction, float32 delta) {
	const static float32 SPEED = 5.0f;

	switch (direction) {
	case NORTH:
		getParentObject()->transform.translate(0, SPEED * delta);
		break;
	case NORTH_WEST:
		getParentObject()->transform.translate(Vector2f::createVectorFromPolar(SPEED * delta, 3 * PI_4));
		break;
	case NORTH_EAST:
		getParentObject()->transform.translate(Vector2f::createVectorFromPolar(SPEED * delta, PI_4));
		break;
	case SOUTH:
		getParentObject()->transform.translate(0, -SPEED * delta);
		break;
	case SOUTH_WEST:
		getParentObject()->transform.translate(Vector2f::createVectorFromPolar(SPEED * delta, 5 * PI_4));
		break;
	case SOUTH_EAST:
		getParentObject()->transform.translate(Vector2f::createVectorFromPolar(SPEED * delta, 7 * PI_4));
		break;
	case WEST:
		getParentObject()->transform.translate(-SPEED * delta, 0);
		break;
	case EAST:
		getParentObject()->transform.translate(SPEED * delta, 0);
		break;
	}
}

void PlayerController::receiveEvent(const Event& event) {

}

void PlayerController::onKey(int32 key, int32 scancode, int32 action, int32 mods) {
	if (key == Keys::KEY_W || key == Keys::KEY_UP) {
		if (action == Actions::PRESS) {
			m_upAction = true;
		}
		if (action == Actions::RELEASE) {
			m_upAction = false;
		}
	}
	
	if (key == Keys::KEY_A || key == Keys::KEY_LEFT) {
		if (action == Actions::PRESS) {
			m_leftAction = true;
		}
		if (action == Actions::RELEASE) {
			m_leftAction = false;
		}
	}
	
	if (key == Keys::KEY_S || key == Keys::KEY_DOWN) {
		if (action == Actions::PRESS) {
			m_downAction = true;
		}
		if (action == Actions::RELEASE) {
			m_downAction = false;
		}
	}
	
	if (key == Keys::KEY_D || key == Keys::KEY_RIGHT) {
		if (action == Actions::PRESS) {
			m_rightAction = true;
		}
		if (action == Actions::RELEASE) {
			m_rightAction = false;
		}
	}

	if (key == Keys::KEY_SPACE) {
		if (action == Actions::PRESS && !m_swingingSword) {
			SpriteComponent* cp = m_player->getSword()->findComponent<SpriteComponent>();
			m_swordCountdown = 0.1f;
			cp->setVisible(true);
			m_swingingSword = true;
			//SoundEngine::get()->playSound("res/sound/swish.wav");
		}
	}
}

#ifdef DEBUG_BUILD
void PlayerController::debugRender() {}
#endif