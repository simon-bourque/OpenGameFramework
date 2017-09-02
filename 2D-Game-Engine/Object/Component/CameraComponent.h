#pragma once

#include "Object/Component/ObjectComponent.h"

#include "Core/Core.h"

class GameObject;
class Camera;

class CameraComponent : public ObjectComponent {
public:
	enum {
		BOUNDED_LEFT_BIT = 0b0001,
		BOUNDED_RIGHT_BIT = 0b0010,
		BOUNDED_TOP_BIT = 0b0100,
		BOUNDED_BOTTOM_BIT = 0b1000,
		BOUNDED_ALL = BOUNDED_LEFT_BIT | BOUNDED_RIGHT_BIT | BOUNDED_TOP_BIT | BOUNDED_BOTTOM_BIT
	};
private:
	Camera* m_camera;

	float32 m_xOffset;
	float32 m_yOffset;

	bool m_trackX : 1;
	bool m_trackY : 1;
	bool m_sceneBounded : 1;
	uint8 m_sceneBoundsMask : 4;
public:
	CameraComponent(GameObject* parentObject, Camera* camera);
	virtual ~CameraComponent();

	virtual void tick(float32 delta) override;
	virtual void receiveEvent(const Event& event) override;

	float32 getXOffset() const { return m_xOffset; };
	float32 getYOffset() const { return m_yOffset; };
	bool isTrackingX() const { return m_trackX; };
	bool isTrackingY() const { return m_trackY; };
	bool isSceneBounded() const { return m_sceneBounded; };

	void setXOffset(float32 xOffset) { m_xOffset = xOffset; };
	void setYOffset(float32 yOffset) { m_yOffset = yOffset; };
	void setTrackX(bool trackX) { m_trackX = trackX; };
	void setTrackY(bool trackY) { m_trackY = trackY; };
	void setSceneBounded(bool sceneBounded) { m_sceneBounded = sceneBounded; };
	void setSceneBounds(uint8 mask) { m_sceneBoundsMask = mask; };

#ifdef DEBUG_BUILD
	virtual void debugRender() override;
#endif

	GET_COMPONENT_TYPE_DECLERATION
};

REGISTER_COMPONENT_TYPE(CameraComponent)