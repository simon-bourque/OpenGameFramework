#pragma once

#include <unordered_map>
#include <string>

#include <GL/glew.h>

#include "3D/Graphics/Shader/ShaderProgramOld.h"

class ShaderCache {
	friend class RenderingContext;
private:
	std::unordered_map<std::string, ShaderProgramOld*> m_shaders;

	ShaderCache();
	virtual ~ShaderCache();

	GLint compileShader(const std::string& source, GLenum type) const;
public:
	
	ShaderProgramOld* loadShaderProgram(const std::string& name, const std::string& vertPath, const std::string& fragPath);
	ShaderProgramOld* loadShaderProgram(const std::string& name, const std::string& vertPath, const std::string& fragPath, const std::string& geoPath);
	ShaderProgramOld* getShaderProgram(const std::string& name) { return m_shaders[name]; };
};

