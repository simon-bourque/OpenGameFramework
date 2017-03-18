#ifndef TEXTURE_H
#define TEXTURE_H

#include <Gl/glew.h>

#include "Core.h"

class RawImage;

class Texture
{

	friend class TextureManager;

public:

	enum Target : GLenum {
		TEXTURE_2D = GL_TEXTURE_2D,
		TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY
	};

	enum Filter : GLint {
		NEAREST_NEIGHBOR = GL_NEAREST,
		LINEAR = GL_LINEAR
	};

	enum Wrap : GLint {
		REPEAT = GL_REPEAT,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
	};

	enum Unit : GLenum {
		UNIT_0 = GL_TEXTURE0
	};

private:

	GLuint m_name;
	Target m_target;
	GLenum m_unit;
	Texture(Target target, GLenum unit);
public:
	virtual ~Texture();

	void bind() const;
	void unbind() const;

	GLuint getName() const { return m_name; };
	Target getTarget() const { return m_target; };
	GLenum getUnit() const { return m_unit; };

	// Prevent copying of texture
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
};

#endif

