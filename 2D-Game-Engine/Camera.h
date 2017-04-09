#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"
#include "Rectangle.h"
#include "Matrix3f.h"

class Camera
{
private:
	Transform m_transform;
	
	Rectangle m_viewPort;

	Matrix3f m_projectionMatrix;
	Matrix3f m_viewProjectionMatrix;
public:
	explicit Camera(const Rectangle& viewPort);
	Camera(const Transform& transform, const Rectangle& viewPort);
	~Camera();

	void updateViewProjectionMatrix();
	Matrix3f getViewMatrix() const;

	const Transform& getTransform() const { return m_transform; }
	Transform& getTransform() { return m_transform; }

	const Matrix3f& getProjectionMatrix() const { return m_projectionMatrix; }
	const Matrix3f& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }
	const Rectangle& getViewPort() const { return m_viewPort; };
};

#endif