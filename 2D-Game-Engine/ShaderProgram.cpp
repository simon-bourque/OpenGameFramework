#include "ShaderProgram.h"



ShaderProgram::ShaderProgram(const string& vertSrc, const string& fragSrc) {
	// TODO handle shader compilation error
	GLint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	compileShader(vertShaderId, vertSrc);
	compileShader(fragShaderId, fragSrc);

	m_programId = glCreateProgram();
	glAttachShader(m_programId, vertShaderId);
	glAttachShader(m_programId, fragShaderId);
	glLinkProgram(m_programId);

	GLint status = 0;
	glGetProgramiv(m_programId, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {
		string msg("Failed to link program: ");

		const static GLsizei logMax = 1024;
		GLchar log[logMax];
		GLsizei charsWritten = 0;
		glGetProgramInfoLog(m_programId, logMax, &charsWritten, log);

		msg.append(log);
		DEBUG_LOG(msg);
	}

	GLint numUniforms = 0;
	glGetProgramiv(m_programId, GL_ACTIVE_UNIFORMS, &numUniforms);

	for (int32 i = 0; i < numUniforms; i++) {

		const static GLsizei nameMax = 1024;
		GLchar name[nameMax];
		GLsizei charsWritten = 0;

		GLint size = 0;
		GLenum type = 0;

		glGetActiveUniform(m_programId, i, nameMax, &charsWritten, &size, &type, name);

		GLint location = glGetUniformLocation(m_programId, name);

		m_uniforms[string(name)] = Uniform(string(name), location, size, type);
	}

	// Clean up.
	glDetachShader(m_programId, vertShaderId);
	glDetachShader(m_programId, fragShaderId);
	glDeleteShader(vertShaderId);
	glDeleteShader(fragShaderId);
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(m_programId);
}

void ShaderProgram::compileShader(GLint shaderID, const string& src) {
	const GLchar* srcCString = src.c_str();

	glShaderSource(shaderID, 1, &srcCString, nullptr);
	glCompileShader(shaderID);

	GLint status = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		string msg("Failed to compile shader: ");

		const static GLsizei logMax = 1024;
		GLchar log[logMax];
		GLsizei charsWritten = 0;
		glGetShaderInfoLog(shaderID, logMax, &charsWritten, log);

		msg.append(log);
		DEBUG_LOG(msg);
	}
}

const Uniform& ShaderProgram::getUniform(const string& name) const {
	// TODO check if uniform actually exists
	return m_uniforms.at(name);
}
