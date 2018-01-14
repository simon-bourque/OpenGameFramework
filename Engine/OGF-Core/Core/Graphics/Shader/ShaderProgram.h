#pragma once

#include "Core/Core.h"

#include "Core/Graphics/Shader/Uniform.h"

#include "Core/Math/Matrix4f.h"
#include "Core/Math/Vector3f.h"

#include <GL/glew.h>

#include <map>
#include <vector>

class ShaderProgram
{
	friend class ShaderCache;
private:
	GLuint m_programId;
	std::map<string, Uniform> m_uniforms;

	void compileShader(GLint shaderID, const string& src);
public:
	ShaderProgram(const string& vertSrc, const string& fragSrc);
	~ShaderProgram();

	GLuint getProgramId() const { return m_programId; };
	const Uniform& getUniform(const string& name) const;

	void use() const;

	void setUniform(const std::string& uniformName, const Matrix4f& matrix);
	void setUniform(const std::string& uniformName, const Vector3f& vector);
	void setUniform(const std::string& uniformName, int32 integer);
	void setUniform(const std::string& uniformName, float32 floatdata);

	void setUniform(const std::string& uniformName, const std::vector<Vector3f>& vectors);

	// Prevent copying of shader program
	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
};
