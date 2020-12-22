#pragma once

#include "2D/Graphics/Camera.h"

#include "Core/Singleton.h"

#include <memory>

class SceneRenderer;
class ShapeRenderer;
class SpriteRenderer;
class EmitterRenderer;

class Graphics2D {
	SINGLETON_DECLARATION(Graphics2D)
private:
	Camera m_camera;

	std::unique_ptr<SceneRenderer> m_sceneRenderer;
	std::unique_ptr<ShapeRenderer> m_shapeRenderer;
	std::unique_ptr<SpriteRenderer> m_spriteRenderer;
	std::unique_ptr<EmitterRenderer> m_emitterRenderer;

	Graphics2D(const Camera& camera);
public:
	virtual ~Graphics2D();

	SceneRenderer& getSceneRenderer() { return *m_sceneRenderer; }
	ShapeRenderer& getShapeRenderer() { return *m_shapeRenderer; }
	SpriteRenderer& getSpriteRenderer() { return *m_spriteRenderer; }
	EmitterRenderer& getEmitterRenderer() { return *m_emitterRenderer; }

	Camera& getCamera() { return m_camera; }
};

SINGLETON_ACCESSOR(Graphics2D)