#pragma once

#include "Core/Core.h"

#include <GL/glew.h>

class Texture
{

	friend class TextureManager;
	friend class TextureCache;

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

	enum class Attachment : GLenum {
		COLOR_ATTACH = GL_COLOR_ATTACHMENT0,
		DEPTH_ATTACH = GL_DEPTH_ATTACHMENT,
		STENCIL_ATTACH = GL_STENCIL_ATTACHMENT
	};

	enum class Unit : GLenum {
		UNIT_0 = GL_TEXTURE0,
		UNIT_1 = GL_TEXTURE1,
		UNIT_2 = GL_TEXTURE2,
		UNIT_3 = GL_TEXTURE3,
		UNIT_4 = GL_TEXTURE4
	};

private:
	GLuint _texid;
	Target _target;
	Texture(Target target);
public:
	virtual ~Texture();

	void bind(Unit unit) const;
	void unbind() const;

	Target getTarget() const { return _target; };
	GLuint getId() const { return _texid; };

	// Prevent copying of texture
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
};