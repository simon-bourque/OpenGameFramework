#include "EmitterComponent.h"

#include "Core/Graphics/GraphicsContext.h"
#include "Core/Graphics/Shader/ShaderProgram.h"
#include "Core/Resource/Resources.h"

#include "2D/Graphics/Graphics2D.h"
#include "2D/Object/GameObject.h"
#include "2D/Graphics/Renderer/EmitterRenderer.h"

#include <algorithm>
#include <stdlib.h>

GET_COMPONENT_TYPE_DEFINITION(EmitterComponent)

#undef min

EmitterComponent::EmitterComponent(
	GameObject* parentObject,
	TextureRef textureRef,
	std::string vertShader,
	std::string fragShader)
	: RenderableComponent(parentObject)
	, m_textureRef(textureRef)
{
	m_particleShader.reset(new ShaderProgram(loadSrc(vertShader), loadSrc(fragShader)));
}

EmitterComponent::~EmitterComponent() {}

void EmitterComponent::setupEmitter(uint32 quantity, float32 lifespan) {
	m_particles.clear();
	m_particles.resize(quantity);

	for (int i = 0; i < m_particles.size(); i++) {
		Particle2D& particle = m_particles.at(i);

		particle.position.x = getParentObject()->transform.xPos;
		particle.position.y = getParentObject()->transform.yPos;

		particle.lifespan = lifespan;

		// We are giving a life depending on the position of the particle in the vector to have a continuous
		// trail of particles.
		particle.life = (particle.lifespan * i) / m_particles.size();

		particle.color = { 1.0f, 1.0f, 1.0f, 0.5f };
	}
}

void EmitterComponent::tick(float32 delta) {
	// TICK EM
	for (int i = 0; i < m_particles.size(); i++) {
		Particle2D& particle = m_particles.at(i);

		if (isVisible())
		{
			particle.life += delta;

			if (particle.life >= particle.lifespan) {
				particle.position.x = getParentObject()->transform.xPos;
				particle.position.y = getParentObject()->transform.yPos;
				particle.life = (particle.lifespan * i) / m_particles.size();
			}

			// Integrate new position
			particle.position.x += delta * particle.velocity.x;
			particle.position.y += delta * particle.velocity.y;

			// Integrate new color
			particle.color.a = 1.0f - std::min(particle.life / particle.lifespan, 1.0f);
		}
		else
		{
			particle.position.x = getParentObject()->transform.xPos;
			particle.position.y = getParentObject()->transform.yPos;
		}
	}
}

void EmitterComponent::render() {
	if (isVisible()) {
		getGraphics2DInstance()->getEmitterRenderer().renderParticles(m_particleShader.get(), m_textureRef, m_particles);
	}
}

void EmitterComponent::receiveEvent(const Event& event) {

}

void EmitterComponent::replaceTexture(std::string newTexture) {
	getGraphicsContextInstance()->getTextureCache()->destroyTexture(m_textureRef);
	m_textureRef = getGraphicsContextInstance()->getTextureCache()->loadTexture(newTexture);
}

#ifdef DEBUG_BUILD
void EmitterComponent::debugRender() {}
#endif