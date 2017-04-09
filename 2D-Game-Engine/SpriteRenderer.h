#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "Renderer.h"
#include "Core.h"

class RenderSystem;
class ShaderProgram;
class VertexArrayObject;
struct Transform;
class Texture;

class SpriteRenderer : public Renderer
{
private:
	ShaderProgram* m_spriteShaderProgram;
	ShaderProgram* m_animSpriteShaderProgram;
	ShaderProgram* m_spriteBatchShaderProgram;

	VertexArrayObject* m_spriteVAO;
public:
	static const float32 SPRITE_VERTS[8];
	static const float32 SPRITE_UV[8];
	static const int32 SPRITE_NUM_VERTICES;

	explicit SpriteRenderer(RenderSystem* rs);
	virtual ~SpriteRenderer();

	void renderSprite(const Transform* transform, const Texture* texture, bool hFlip, bool vFlip) const;
};

#endif;

