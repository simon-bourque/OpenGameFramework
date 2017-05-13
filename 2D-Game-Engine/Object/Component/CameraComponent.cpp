#include "CameraComponent.h"

#include "Graphics/Camera.h"

#include "Object/GameObject.h"
#include "Object/Transform.h"

#include "Math/Geometry/Rectangle.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"

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
		m_camera->getTransform().xPos = m_xOffset + getParentObject()->getTransform().xPos;
	}

	if (m_trackY) {
		m_camera->getTransform().yPos = m_yOffset + getParentObject()->getTransform().yPos;
	}

	if (m_sceneBounded) {
		//Rectangle bounds = game.getScene().getBounds();
		const Rectangle& bounds = SceneManager::get()->getCurrentScene().getBounds();
		const Rectangle& viewPort = m_camera->getViewPort();

		float32 cameraX = m_camera->getTransform().xPos;
		float32 cameraY = m_camera->getTransform().yPos;

		float32 leftBound = bounds.getX() - bounds.getHalfWidth();
		float32 rightBound = bounds.getX() + bounds.getHalfWidth();
		float32 topBound = bounds.getY() + bounds.getHalfHeight();
		float32 bottomBound = bounds.getY() - bounds.getHalfHeight();

		// Left bound
		if (cameraX - viewPort.getHalfWidth() < leftBound && (m_sceneBoundsMask & BOUNDED_LEFT_BIT)) {
			m_camera->getTransform().xPos = leftBound + viewPort.getHalfWidth();
		}

		// Right bound
		if (cameraX + viewPort.getHalfWidth() > rightBound && (m_sceneBoundsMask & BOUNDED_RIGHT_BIT)) {
			m_camera->getTransform().xPos = rightBound - viewPort.getHalfWidth();
		}

		// Top bound
		if (cameraY + viewPort.getHalfHeight() > topBound && (m_sceneBoundsMask & BOUNDED_TOP_BIT)) {
			m_camera->getTransform().yPos = topBound - viewPort.getHalfHeight();
		}

		// Bottom bound
		if (cameraY - viewPort.getHalfHeight() < bottomBound && (m_sceneBoundsMask & BOUNDED_BOTTOM_BIT)) {
			m_camera->getTransform().yPos = bottomBound + viewPort.getHalfHeight();
		}
	}
}

void CameraComponent::receiveEvent(const Event& event) {}