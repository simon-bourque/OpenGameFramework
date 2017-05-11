#include "RenderSystem.h"

#include "Graphics/Renderer/SpriteRenderer.h"
#include "Graphics/Renderer/SceneRenderer.h"
#include "Graphics/Renderer/TextRenderer.h"
#include "Graphics/Renderer/ShapeRenderer.h"
#include "Graphics/Text/FontManager.h"
#include "Graphics/Text/TextManager.h"
#include "Graphics/TextureManager.h"

#include "Resource/RawImage.h"

#include <GL/glew.h>

#include <sstream>

RenderSystem::RenderSystem(const Camera& camera) : m_camera(camera) {
	
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

	m_spriteRenderer.reset(new SpriteRenderer(this));
	m_sceneRenderer.reset(new SceneRenderer(this));
	m_textRenderer.reset(new TextRenderer(this));
	m_shapeRenderer.reset(new ShapeRenderer(this));

	m_textureManager.reset(new TextureManager(this));
	m_fontManager.reset(new FontManager(this));
	m_textManager.reset(new TextManager(this));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


RenderSystem::~RenderSystem() {
	glUseProgram(0);
}
