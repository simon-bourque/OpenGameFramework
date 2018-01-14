#include "CameraComponent.h"

#include "2D/Graphics/Camera.h"

#include "2D/Object/GameObject.h"
#include "2D/Object/Transform.h"

#include "Core/Math/Geometry/Rectangle.h"

#include "2D/Scene/SceneManager2D.h"
#include "2D/Scene/Scene2D.h"

GET_COMPONENT_TYPE_DEFINITION(CameraComponent)

CameraComponent::CameraComponent(GameObject* parentObject, Camera* camera) : 
	ObjectComponent(parentObject),
	m_camera(camera),
	m_xOffset(0.0f),
	m_yOffset(0.0f),
	m_trackX(true),
	m_trackY(true),
	m_sceneBounded(false),
	m_sceneBoundsMask(BOUNDED_ALL)
{}

CameraComponent::~CameraComponent() {}

void CameraComponent::tick(float32 delta) {
	if (m_trackX) {
		m_camera->transform.xPos = m_xOffset + getParentObject()->transform.xPos;
	}

	if (m_trackY) {
		m_camera->transform.yPos = m_yOffset + getParentObject()->transform.yPos;
	}

	if (m_sceneBounded) {
		//Rectangle bounds = game.getScene().getBounds();
		const geo::Rectangle& bounds = getSceneManager2DInstance()->getCurrentScene().getBounds();
		const geo::Rectangle& viewPort = m_camera->getViewPort();

		float32 cameraX = m_camera->transform.xPos;
		float32 cameraY = m_camera->transform.yPos;

		float32 leftBound = bounds.getX() - bounds.getHalfWidth();
		float32 rightBound = bounds.getX() + bounds.getHalfWidth();
		float32 topBound = bounds.getY() + bounds.getHalfHeight();
		float32 bottomBound = bounds.getY() - bounds.getHalfHeight();

		// Left bound
		if (cameraX - viewPort.getHalfWidth() < leftBound && (m_sceneBoundsMask & BOUNDED_LEFT_BIT)) {
			m_camera->transform.xPos = leftBound + viewPort.getHalfWidth();
		}

		// Right bound
		if (cameraX + viewPort.getHalfWidth() > rightBound && (m_sceneBoundsMask & BOUNDED_RIGHT_BIT)) {
			m_camera->transform.xPos = rightBound - viewPort.getHalfWidth();
		}

		// Top bound
		if (cameraY + viewPort.getHalfHeight() > topBound && (m_sceneBoundsMask & BOUNDED_TOP_BIT)) {
			m_camera->transform.yPos = topBound - viewPort.getHalfHeight();
		}

		// Bottom bound
		if (cameraY - viewPort.getHalfHeight() < bottomBound && (m_sceneBoundsMask & BOUNDED_BOTTOM_BIT)) {
			m_camera->transform.yPos = bottomBound + viewPort.getHalfHeight();
		}
	}
}

void CameraComponent::receiveEvent(const Event& event) {}

#ifdef DEBUG_BUILD
void CameraComponent::debugRender() {}
#endif