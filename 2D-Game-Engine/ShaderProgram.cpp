#include "ShaderProgram.h"



ShaderProgram::ShaderProgram(const string& vertSrc, const string& fragSrc) {
	// TODO handle shader compilation error
	GLint vertShaderId = glCreateShader(GL_VERTEX_SHADER);
	
	const GLchar* vertSrcPtr = vertSrc.c_str();
	
	glShaderSource(vertShaderId, 1, &vertSrcPtr, nullptr);
	glCompileShader(vertShaderId);

	GLint status = 0;
	glGetShaderiv(vertShaderId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		string msg("Failed to compile shader: ");

		const static GLsizei logMax = 1024;
		GLchar log[logMax];
		GLsizei charsWritten = 0;
		glGetShaderInfoLog(vertShaderId, logMax, &charsWritten, log);
		
		msg.append(log);
		DEBUG_LOG(msg);
	}

	GLint fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	
	const GLchar* fragSrcPtr = fragSrc.c_str();

	glShaderSource(fragShaderId, 1, &fragSrcPtr, nullptr);
	glCompileShader(fragShaderId);

	glGetShaderiv(fragShaderId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) {
		string msg("Failed to compile shader: ");

		const static GLsizei logMax = 1024;
		GLchar log[logMax];
		GLsizei charsWritten = 0;
		glGetShaderInfoLog(fragShaderId, logMax, &charsWritten, log);

		msg.append(log);
		DEBUG_LOG(msg);
	}

	m_programId = glCreateProgram();
	glAttachShader(m_programId, vertShaderId);
	glAttachShader(m_programId, fragShaderId);
	glLinkProgram(m_programId);

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
		//IntBuffer size = BufferUtils.createIntBuffer(1);
		//IntBuffer type = BufferUtils.createIntBuffer(1);
		//String name = GL20.glGetActiveUniform(programId, i, size, type);
		//int location = GL20.glGetUniformLocation(programId, name);
		//uniforms.put(name, new Uniform(name, location, size.get(0), type.get(0)));
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

const Uniform& ShaderProgram::getUniform(const string& name) const {
	// TODO check if uniform actually exists
	return m_uniforms.at(name);
}
