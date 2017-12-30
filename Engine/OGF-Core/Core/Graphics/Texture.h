#pragma once

#include "Core/Core.h"

#include <GL/glew.h>

class RawImage;

class Texture
{

	friend class TextureManager;

public:

	enum class Target : GLenum {
		TEXTURE_2D = GL_TEXTURE_2D,
		TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
		TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
		TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP
	};

	enum class Filter : GLint {
		NEAREST_NEIGHBOR = GL_NEAREST,
		LINEAR = GL_LINEAR
	};

	enum class Wrap : GLint {
		REPEAT = GL_REPEAT,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
	};

	enum class Unit : GLenum {
		UNIT_0 = GL_TEXTURE0,
		UNIT_1 = GL_TEXTURE1,
		UNIT_2 = GL_TEXTURE2,
		UNIT_3 = GL_TEXTURE3,
		UNIT_4 = GL_TEXTURE4
	};

private:
	GLuint m_name;
	Target m_target;
	Texture(Target target);
public:
	virtual ~Texture();

	void bind(Unit unit) const;
	void unbind() const;

	GLuint getName() const { return m_name; };
	Target getTarget() const { return m_target; };

	// Prevent copying of texture
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
};