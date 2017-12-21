#pragma once

#include "Core/Core.h"

#include <memory>

class ShaderProgram;
class VertexArrayObject;
struct Transform;
class Texture;

namespace geo {
	class Rectangle;
}

class SpriteRenderer {
private:
	std::unique_ptr<ShaderProgram> m_spriteShaderProgram;
	std::unique_ptr<ShaderProgram> m_animSpriteShaderProgram;
	std::unique_ptr<ShaderProgram> m_spriteBatchShaderProgram;

	std::unique_ptr<VertexArrayObject> m_spriteVAO;
public:
	static const float32 SPRITE_VERTS[8];
	static const float32 SPRITE_UV[8];
	static const int32 SPRITE_NUM_VERTICES;

	explicit SpriteRenderer();
	virtual ~SpriteRenderer();

	void renderSprite(const Transform* transform, const Texture* texture, bool hFlip, bool vFlip) const;
	void renderSpriteUI(const geo::Rectangle& bounds, const Texture* texture, bool hFlip, bool vFlip) const;
	void renderAnimationFrame(const Transform& transform, uint32 frame, const Texture& texture, bool hFlip, bool vFlip) const;
};