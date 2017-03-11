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
	glBindBuffer(m_target, m_handle);
}

void Buffer::unbind() const {
	glBindBuffer(m_target, 0);
}

void Buffer::bufferData(GLvoid* data, GLsizeiptr size, Usage usage) {
	glBufferData(m_target, size, data, usage);
}

void Buffer::vertexAttributePointer(GLuint index, GLint vertexSize, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer) {
	glVertexAttribPointer(index, vertexSize, type, normalized, stride, pointer);
}
