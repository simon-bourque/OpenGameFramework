#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "Core/Types.h"

class ShaderProgramOld {
	friend class ShaderCache;
private:
	GLuint m_shaderHandle;

	ShaderProgramOld(GLuint shaderHandle);
	virtual ~ShaderProgramOld();
public:
	ShaderProgramOld(const ShaderProgramOld&) = delete;
	ShaderProgramOld& operator=(const ShaderProgramOld&) = delete;

	void use() const;

	void setUniform(const std::string& uniformName, const glm::mat4& matrix);
	void setUniform(const std::string& uniformName, const glm::vec3& vector);
	void setUniform(const std::string& uniformName, int32 integer);
	void setUniform(const std::string& uniformName, float32 floatdata);

	void setUniform(const std::string& uniformName, const std::vector<glm::vec3>& vectors);
};

