#pragma once

#include "2D/Object/Transform.h"

#include "Core/Math/Geometry/Rectangle.h"
#include "Core/Math/Matrix3f.h"

class Camera
{
public:
	Transform transform;
private:	
	geo::Rectangle m_viewPort;

	Matrix3f m_projectionMatrix;
	Matrix3f m_viewProjectionMatrix;
public:
	explicit Camera(const geo::Rectangle& viewPort);
	Camera(const Transform& transform, const geo::Rectangle& viewPort);
	~Camera();

	void updateViewProjectionMatrix();
	Matrix3f getViewMatrix() const;

	const Matrix3f& getProjectionMatrix() const { return m_projectionMatrix; }
	const Matrix3f& getViewProjectionMatrix() const { return m_viewProjectionMatrix; }
	const geo::Rectangle& getViewPort() const { return m_viewPort; };
};