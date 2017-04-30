#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "Core.h"
#include "Uniform.h"

#include "GL/glew.h"
#include <map>

class ShaderProgram
{
private:
	GLuint m_programId;
	std::map<string, Uniform> m_uniforms;

	void compileShader(GLint shaderID, const string& src);
public:
	ShaderProgram(const string& vertSrc, const string& fragSrc);
	~ShaderProgram();

	GLuint getProgramId() const { return m_programId; };
	const Uniform& getUniform(const string& name) const;

	// Prevent copying of shader program
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
};

#endif

