#ifndef BUFFER_H
#define BUFFER_H

#include "Core.h"

#include <GL/glew.h>

class Buffer
{
public:
	enum class Target : GLenum {
		ARRAY_BUFFER = GL_ARRAY_BUFFER,
		ATOMIC_COUNTER_BUFFER = GL_ATOMIC_COUNTER_BUFFER,
		COPY_READ_BUFFER = GL_COPY_READ_BUFFER,
		COPY_WRITE_BUFFER = GL_COPY_WRITE_BUFFER,
		DISPATCH_INDIRECT_BUFFER = GL_DISPATCH_INDIRECT_BUFFER,
		DRAW_INDIRECT_BUFFER = GL_DRAW_INDIRECT_BUFFER,
		ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
		PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
		PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
		QUERY_BUFFER = GL_QUERY_BUFFER,
		SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER,
		TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
		TRANSFORM_FEEDBACK_BUFFER = GL_TRANSFORM_FEEDBACK_BUFFER,
		UNIFORM_BUFFER = GL_UNIFORM_BUFFER
	};

	enum class Usage : GLenum {
		STREAM_DRAW = GL_STREAM_DRAW,
		STREAM_READ = GL_STREAM_READ,
		STREAM_COPY = GL_STREAM_COPY,
		STATIC_DRAW = GL_STATIC_DRAW,
		STATIC_READ = GL_STATIC_READ,
		STATIC_COPY = GL_STATIC_COPY,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
		DYNAMIC_READ = GL_DYNAMIC_READ,
		DYNAMIC_COPY = GL_DYNAMIC_COPY
	};
private:
	GLuint m_handle;
	Target m_target;
public:
	explicit Buffer(Target target);
	virtual ~Buffer();

	void bufferData(const GLvoid* data, GLsizeiptr size, Usage usage);
	void vertexAttributePointer(GLuint index, GLint vertexSize, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer);
	void vertexAttributeIPointer(GLuint index, GLint vertexSize, GLenum type, GLsizei stride, GLvoid* pointer);
	void vertexAttributeDivisor(GLuint index, GLuint divisor);

	void bind() const;
	void unbind() const;

#ifdef DEBUG_BUILD
	void setDebugLabel(const string& label);
#endif

	// Prevent copying of buffer
	Buffer(const Buffer&) = delete;
	Buffer& operator=(const Buffer&) = delete;
};

#endif