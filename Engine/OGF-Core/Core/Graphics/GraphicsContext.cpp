#include "GraphicsContext.h"

#include "Core/Graphics/Renderer/TextRenderer.h"

#include "Core/Graphics/Text/FontManager.h"
#include "Core/Graphics/Text/TextManager.h"
#include "Core/Graphics/TextureCache.h"

#include "Core/Graphics/Color.h"

#include <GL/glew.h>

#include <sstream>

GraphicsContext::GraphicsContext() {
	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Failed to initialize GLEW.");
	}

	DEBUG_LOG(string((char*)glGetString(GL_VERSION)));

	glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)-> void {
	
		string decodedMessage(message, length);
		string decodedSeverity;

		switch (severity) {
		case GL_DEBUG_SEVERITY_LOW:
			decodedSeverity = "LOW";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			decodedSeverity = "MEDIUM";
			break;
		case GL_DEBUG_SEVERITY_HIGH:
			decodedSeverity = "HIGH";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			decodedSeverity = "NOTIFY";
			break;
		default:
			decodedSeverity = "?";
		}

		std::stringstream ss;
		ss << "[" << decodedSeverity << "] " << decodedMessage;
		DEBUG_LOG(ss.str());

	}, nullptr);

	const char* msg = "This is a test message!";
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, 1, GL_DEBUG_SEVERITY_NOTIFICATION, 24, msg);
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//GL11.glEnable(GL13.GL_MULTISAMPLE);
	//glClearColor(0.0f, 0.0f,1.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//glPixelStorei(GL_PACK_ALIGNMENT, 1);
	m_textRenderer.reset(new TextRenderer());

	m_textureCache.reset(new TextureCache());
	m_fontManager.reset(new FontManager());
	m_textManager.reset(new TextManager());

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


GraphicsContext::~GraphicsContext() {
	glUseProgram(0);
}

void GraphicsContext::setClearColor(const Color& color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void GraphicsContext::setClearColor(float32 r, float32 g, float32 b, float32 a) {
	// TODO clamp or assert channels being between 0.0f and 1.0f
	glClearColor(r, g, b, a);
}