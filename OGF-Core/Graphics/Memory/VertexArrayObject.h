#pragma once

#include "Core/Core.h"

#include "Graphics/Memory/Buffer.h"

#include <GL/glew.h>
#include <vector>

class VertexArrayObject
{
private:
	GLuint m_handle;
	std::vector<Buffer*> m_buffers;
public:
	VertexArrayObject();
	virtual ~VertexArrayObject();

	void addArrayBuffer(GLuint attribIndex, const GLvoid* data, GLsizeiptr dataSize, GLint vertexSize, GLenum type, Buffer::Usage usage);
	void addArrayBuffer(Buffer* buffer);

	void enableAttribute(GLuint index);
	void disableAttribute(GLuint index);

	void bind() const;
	void unbind() const;

	Buffer* getBuffer(uint32 index) const { return m_buffers[index]; }; // TODO add bounds checking
};