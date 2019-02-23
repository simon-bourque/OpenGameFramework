#pragma once

#include "Core/Core.h"

#include <GL/glew.h>

class Texture
{

	friend class TextureCache;

public:

	enum class Target : GLenum {
		TEXTURE_2D = GL_TEXTURE_2D,
		TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
		TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
		TEXTURE_CUBE_MAP = GL_TEXTURE_CUBE_MAP
	};

	enum class InternalFormat : GLenum {
		RGBA = GL_RGBA,
		RGB = GL_RGB,
		DEPTH_COMP = GL_DEPTH_COMPONENT,
		DEPTH_STENCIL = GL_DEPTH_STENCIL
	};

	enum class Format : GLenum {
		RGBA = GL_RGBA,
		RGB = GL_RGB,
		DEPTH_COMP = GL_DEPTH_COMPONENT,
		DEPTH_STENCIL = GL_DEPTH_STENCIL
	};

	enum class PixelDataType : GLenum {
		UBYTE = GL_UNSIGNED_BYTE,
		BYTE = GL_BYTE,
		USHORT = GL_UNSIGNED_SHORT,
		SHORT = GL_SHORT,
		UINT = GL_UNSIGNED_INT,
		INT = GL_INT,
		HALF_FLOAT = GL_HALF_FLOAT,
		FLOAT = GL_FLOAT
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
	uint32 _texid;
	Target _target;
	InternalFormat _internalFormat;
	Format _format;
	PixelDataType _type;

	Texture(Target target, InternalFormat internalFormat, Format format, PixelDataType type);
public:
	virtual ~Texture();

	void bind(Unit unit) const;
	void unbind() const;
	void resize(const int32 width, const int32 height);

	Target getTarget() const { return _target; };
	uint32 getId() const { return _texid; };

	// Prevent copying of texture
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
};