#pragma once
#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "Graphics/Renderer/Renderer.h"
#include "Core/Core.h"

#include <memory>

class RenderSystem;
class ShaderProgram;
class VertexArrayObject;
struct Transform;
class Texture;
class Rectangle;

class SpriteRenderer : public Renderer {
private:
	std::unique_ptr<ShaderProgram> m_spriteShaderProgram;
	std::unique_ptr<ShaderProgram> m_animSpriteShaderProgram;
	std::unique_ptr<ShaderProgram> m_spriteBatchShaderProgram;

	std::unique_ptr<VertexArrayObject> m_spriteVAO;
public:
	static const float32 SPRITE_VERTS[8];
	static const float32 SPRITE_UV[8];
	static const int32 SPRITE_NUM_VERTICES;

	explicit SpriteRenderer(RenderSystem* rs);
	virtual ~SpriteRenderer();

	void renderSprite(const Transform* transform, const Texture* texture, bool hFlip, bool vFlip) const;
	void renderSpriteUI(const Rectangle& bounds, const Texture* texture, bool hFlip, bool vFlip) const;
	void renderAnimationFrame(const Transform& transform, uint32 frame, const Texture& texture, bool hFlip, bool vFlip) const;
};

#endif;

