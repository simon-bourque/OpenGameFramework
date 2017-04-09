#include "RenderSystem.h"

#include <GL/glew.h>

#include "SpriteRenderer.h"
#include "SceneRenderer.h"
#include "RawImage.h"
#include "TextureManager.h"

#include <sstream>

RenderSystem::RenderSystem(const Camera& camera) : m_camera(camera) {
	
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		throw runtime_error("Failed to initialize GLEW.");
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

		stringstream ss;
		ss << "[" << decodedSeverity << "] " << decodedMessage;
		DEBUG_LOG(ss.str());

	}, nullptr);

	const char* msg = "This is a test message!";
	glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, 1, GL_DEBUG_SEVERITY_NOTIFICATION, 24, msg);
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//GL11.glEnable(GL13.GL_MULTISAMPLE);
	glClearColor(0.0f, 0.0f,1.0f, 1.0f);

	m_spriteRenderer = new SpriteRenderer(this);
	m_sceneRenderer = new SceneRenderer(this);

	m_textureManager = new TextureManager();
}


RenderSystem::~RenderSystem() {
	glUseProgram(0);
	delete m_spriteRenderer;
	delete m_sceneRenderer;
	delete m_textureManager;
}
