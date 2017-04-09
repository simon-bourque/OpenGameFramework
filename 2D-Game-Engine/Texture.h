#ifndef TEXTURE_H
#define TEXTURE_H

#include <Gl/glew.h>

#include "Core.h"

class RawImage;

class Texture
{

	friend class TextureManager;

public:

	enum class Target : GLenum {
		TEXTURE_2D = GL_TEXTURE_2D,
		TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY
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
		UNIT_0 = GL_TEXTURE0
	};

private:

	GLuint m_name;
	Target m_target;
	Unit m_unit;
	Texture(Target target, Unit unit);
public:
	virtual ~Texture();

	void bind() const;
	void unbind() const;

	GLuint getName() const { return m_name; };
	Target getTarget() const { return m_target; };
	Unit getUnit() const { return m_unit; };

	// Prevent copying of texture
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
};

#endif

