#include "VertexArrayObject.h"


VertexArrayObject::VertexArrayObject() {
	glGenVertexArrays(1, &m_handle);
}


VertexArrayObject::~VertexArrayObject() {
	for (Buffer* buffer : m_buffers) {
		delete buffer;
	}

	bind();
	glDeleteVertexArrays(1, &m_handle);
	m_handle = 0;
	unbind();
}

void VertexArrayObject::addArrayBuffer(GLuint attribIndex, const GLvoid* data, GLsizeiptr dataSize, GLint vertexSize, GLenum type, Buffer::Usage usage) {
	Buffer* buffer = new Buffer(Buffer::Target::ARRAY_BUFFER);

	buffer->bind();
	buffer->bufferData(data, dataSize, usage);
	buffer->vertexAttributePointer(attribIndex, vertexSize, type, false, 0, nullptr);
	buffer->unbind();

	m_buffers.push_back(buffer);
	enableAttribute(attribIndex);
}

void VertexArrayObject::addArrayBuffer(Buffer* buffer) {
	m_buffers.push_back(buffer);
}

void VertexArrayObject::enableAttribute(GLuint index) {
	glEnableVertexAttribArray(index);
}

void VertexArrayObject::disableAttribute(GLuint index) {
	glDisableVertexAttribArray(index);
}

void VertexArrayObject::bind() const {
	glBindVertexArray(m_handle);
}

void VertexArrayObject::unbind() const {
	glBindVertexArray(0);
}
