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

void Buffer::bufferData(GLsizeiptr size, Usage usage) {
	glBufferData(static_cast<GLenum>(m_target), size, nullptr, static_cast<GLenum>(usage));
}

void Buffer::bufferData(const GLvoid* data, GLsizeiptr size, Usage usage) {
	glBufferData(static_cast<GLenum>(m_target), size, data, static_cast<GLenum>(usage));
}

void Buffer::bufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid * data) {
	glBufferSubData(static_cast<GLenum>(m_target), offset, size, data);
}

void Buffer::vertexAttributePointer(GLuint index, GLint vertexSize, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer) {
	glVertexAttribPointer(index, vertexSize, type, normalized, stride, pointer);
}

void Buffer::vertexAttributeIPointer(GLuint index, GLint vertexSize, GLenum type, GLsizei stride, GLvoid* pointer) {
	glVertexAttribIPointer(index, vertexSize, type, stride, pointer);
}

void Buffer::vertexAttributeDivisor(GLuint index, GLuint divisor) {
	glVertexAttribDivisor(index, divisor);
}

#ifdef DEBUG_BUILD
void Buffer::setDebugLabel(const string& label) {
	glObjectLabel(GL_BUFFER, m_handle, label.length(), label.c_str());
}
#endif
