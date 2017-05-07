#include "Camera.h"

Camera::Camera(const Rectangle& viewPort) : Camera(Transform(), viewPort) {}

Camera::Camera(const Transform& transform, const Rectangle& viewPort) : m_transform(transform), m_viewPort(viewPort) {
	m_projectionMatrix = Matrix3f::orthographic(-viewPort.getHalfWidth(), viewPort.getHalfWidth(), viewPort.getHalfHeight(), -viewPort.getHalfHeight(), 1, 1000);
}

Camera::~Camera() {}

void Camera::updateViewProjectionMatrix() {
	m_viewProjectionMatrix = m_projectionMatrix * getViewMatrix();
}

Matrix3f Camera::getViewMatrix() const {
	return m_transform.toMatrix().inverse();
}