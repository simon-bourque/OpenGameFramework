#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H

#include "Core.h"

#include <GL/glew.h>
#include <vector>

#include "Buffer.h"

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
};

#endif

