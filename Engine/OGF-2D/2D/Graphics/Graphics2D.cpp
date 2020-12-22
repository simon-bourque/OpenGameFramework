#include "Graphics2D.h"

#include "2D/Graphics/Renderer/SceneRenderer.h"
#include "2D/Graphics/Renderer/ShapeRenderer.h"
#include "2D/Graphics/Renderer/SpriteRenderer.h"
#include "2D/Graphics/Renderer/EmitterRenderer.h"

Graphics2D::Graphics2D(const Camera& camera) : m_camera(camera) {
	m_sceneRenderer.reset(new SceneRenderer());
	m_shapeRenderer.reset(new ShapeRenderer());
	m_spriteRenderer.reset(new SpriteRenderer());
	m_emitterRenderer.reset(new EmitterRenderer());
}


Graphics2D::~Graphics2D() {}
