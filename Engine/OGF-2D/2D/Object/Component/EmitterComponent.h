#pragma once

#include "Core/Core.h"
#include "Core/Graphics/TextureCache.h"
#include "Core/Math/Vector2f.h"
#include "2D/Graphics/Particle2D.h"

#include "2D/Object/Component/RenderableComponent.h"

#include <string>

class ShaderProgram;

class EmitterComponent : public RenderableComponent {
private:
	TextureRef m_textureRef;
	std::unique_ptr<ShaderProgram> m_particleShader;

	std::vector<Particle2D> m_particles;

	Vector2f m_position;

public:
	EmitterComponent(GameObject* parentObject, TextureRef textureRef, std::string vertShader, std::string fragShader);
	virtual ~EmitterComponent();

	void setupEmitter(uint32 quantity, float32 lifespan);

	void replaceTexture(std::string newTexture);

	virtual void tick(float32 delta) override;
	virtual void render() override;

	virtual void receiveEvent(const Event& event) override;

#ifdef DEBUG_BUILD
	virtual void debugRender() override;
#endif

	GET_COMPONENT_TYPE_DECLERATION
};

REGISTER_COMPONENT_TYPE(EmitterComponent)