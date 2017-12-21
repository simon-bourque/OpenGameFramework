#pragma once

#include "Core/Core.h"

#include <GL/glew.h>

class Uniform
{
private:
	string m_name;
	GLint m_location;
	GLint m_size;
	GLenum m_type;
public:
	Uniform();
	Uniform(const string& name, GLint location, GLint size, GLenum type);
	~Uniform();

	string getName() const { return m_name; };
	GLint getLocation() const { return m_location; };
	GLint getSize() const { return m_size; };
	GLenum getType() const { return m_type; };
};