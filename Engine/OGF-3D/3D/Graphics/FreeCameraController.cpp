#include "FreeCameraController.h"

#include "Camera.h"
#include "Core/Input/Input.h"

#include <glm/glm.hpp>

glm::vec2 getMouseDelta();

FreeCameraController::FreeCameraController(Camera* camera) : 
	m_camera(camera),
	m_forwardPressed(false),
	m_backwardPressed(false),
	m_leftPressed(false),
	m_rightPressed(false),
	m_upPressed(false),
	m_downPressed(false),
	m_rollLeftPressed(false),
	m_rollRightPressed(false),
	m_rightMouseButtonPressed(false)
{
	getInputInstance()->addKeyListener(KeyListener::create<FreeCameraController, &FreeCameraController::onKey>(this));
	getInputInstance()->addMouseButtonListener(MouseButtonListener::create<FreeCameraController, &FreeCameraController::onMouseButton>(this));
}

FreeCameraController::~FreeCameraController() {}

void FreeCameraController::update(float32 deltaSeconds) {
	const static float32 SPEED = 100.0f;
	const static float32 ROTATE_SPEED = 0.01f;

	float32 dx = 0;
	float32 dy = 0;
	float32 dz = 0;
	
	if (m_forwardPressed) {
		--dz;
	}
	if (m_backwardPressed) {
		++dz;
	}
	if (m_leftPressed) {
		--dx;
	}
	if (m_rightPressed) {
		++dx;
	}
	if (m_upPressed) {
		++dy;
	}
	if (m_downPressed) {
		--dy;
	}

	glm::vec3 deltaPos(dx,dy,dz);
	deltaPos = (glm::normalize(deltaPos) * SPEED * deltaSeconds);

	if (dx != 0 || dy != 0 || dz != 0) {
		m_camera->transform.translateLocal(deltaPos.x, deltaPos.y, deltaPos.z);
	}

	// ###############################
	glm::vec2 mouseAxis = getMouseDelta() * ROTATE_SPEED;
	if (m_rightMouseButtonPressed) {

		if (abs(mouseAxis.x) > abs(mouseAxis.y)) {
			mouseAxis.y = 0;
		}
		else {
			mouseAxis.x = 0;
		}

		m_camera->transform.rotateLocal(mouseAxis.y, 0, 0);
		m_camera->transform.rotate(0, -mouseAxis.x, 0);
	}

	float32 roll = 0;
	if (m_rollLeftPressed) {
		++roll;
	}
	if (m_rollRightPressed) {
		--roll;
	}

	if (roll != 0) {
		m_camera->transform.rotateLocal(0, 0, roll * ROTATE_SPEED * 100 * deltaSeconds);
	}
}

void FreeCameraController::onKey(int32 key, int32 scancode, int32 action, int32 mods) {
	if (key == Keys::KEY_W) {
		if (action == Actions::PRESS) {
			m_forwardPressed = true;
		}
		else if (action == Actions::RELEASE) {
			m_forwardPressed = false;
		}
	}
	if (key == Keys::KEY_S) {
		if (action == Actions::PRESS) {
			m_backwardPressed = true;
		}
		else if (action == Actions::RELEASE) {
			m_backwardPressed = false;
		}
	}
	if (key == Keys::KEY_A) {
		if (action == Actions::PRESS) {
			m_leftPressed = true;
		}
		else if (action == Actions::RELEASE) {
			m_leftPressed = false;
		}
	}
	if (key == Keys::KEY_D) {
		if (action == Actions::PRESS) {
			m_rightPressed = true;
		}
		else if (action == Actions::RELEASE) {
			m_rightPressed = false;
		}
	}
	if (key == Keys::KEY_SPACE) {
		if (action == Actions::PRESS) {
			m_upPressed = true;
		}
		else if (action == Actions::RELEASE) {
			m_upPressed = false;
		}
	}
	if (key == Keys::KEY_LEFT_CONTROL) {
		if (action == Actions::PRESS) {
			m_downPressed = true;
		}
		else if (action == Actions::RELEASE) {
			m_downPressed = false;
		}
	}
	if (key == Keys::KEY_Q) {
		if (action == Actions::PRESS) {
			m_rollLeftPressed = true;
		}
		else if (action == Actions::RELEASE) {
			m_rollLeftPressed = false;
		}
	}
	if (key == Keys::KEY_E) {
		if (action == Actions::PRESS) {
			m_rollRightPressed = true;
		}
		else if (action == Actions::RELEASE) {
			m_rollRightPressed = false;
		}
	}
}

void FreeCameraController::onMouseButton(int32 button, int32 action, int32 mods) {
	if (button == MouseButtons::MOUSE_BUTTON_RIGHT) {
		if (action == Actions::PRESS) {
			m_rightMouseButtonPressed = true;
		}
		else if (action == Actions::RELEASE) {
			m_rightMouseButtonPressed = false;
		}
	}
}