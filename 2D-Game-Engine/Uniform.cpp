#include "Uniform.h"

Uniform::Uniform() : m_location(0), m_size(0), m_type(0) {}

Uniform::Uniform(const string& name, GLint location, GLint size, GLenum type) : m_name(name), m_location(location), m_size(size), m_type(type) {}


Uniform::~Uniform() {}
