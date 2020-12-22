#pragma once

#include "Core/Core.h"
#include "Core/Graphics/TextureCache.h"

#include "2D/Graphics/Particle2D.h"

#include <memory>

class VertexArrayObject;
class ShaderProgram;

class EmitterRenderer {
private:

	std::unique_ptr<VertexArrayObject> m_particlesVAO;

public:

	static const float32 QUAD_VERTS[8];
	static const float32 QUAD_UV[8];
	static const int32 QUAD_NUM_VERTICES;

	explicit EmitterRenderer();
	virtual ~EmitterRenderer();

	void renderParticles(const ShaderProgram* shader, TextureRef textureRef, const std::vector<Particle2D>& particles);
};