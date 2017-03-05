#include "Camera.h"

Camera::Camera(const Rectangle& viewPort) : m_viewPort(viewPort) {}

Camera::Camera(const Transform& transform, const Rectangle& viewPort) : m_transform(transform), m_viewPort(viewPort) {}

Camera::~Camera() {}

void Camera::updateViewProjectionMatrix() {
	m_viewProjectionMatrix = m_projectionMatrix * getViewMatrix();
}

Matrix3f Camera::getViewMatrix() const {
	return m_transform.toMatrix().inverse();
}