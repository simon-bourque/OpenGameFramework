#include "Buffer.h"



Buffer::Buffer(Target target) : m_target(target) {
	glGenBuffers(1, &m_handle);
}


Buffer::~Buffer() {
	bind();
	glDeleteBuffers(1, &m_handle);
	m_handle = 0;
	unbind();
}

void Buffer::bind() const {
	glBindBuffer(static_cast<GLenum>(m_target), m_handle);
}

void Buffer::unbind() const {
	glBindBuffer(static_cast<GLenum>(m_target), 0);
}

void Buffer::bufferData(const GLvoid* data, GLsizeiptr size, Usage usage) {
	glBufferData(static_cast<GLenum>(m_target), size, data, static_cast<GLenum>(usage));
}

void Buffer::vertexAttributePointer(GLuint index, GLint vertexSize, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer) {
	glVertexAttribPointer(index, vertexSize, type, normalized, stride, pointer);
}
