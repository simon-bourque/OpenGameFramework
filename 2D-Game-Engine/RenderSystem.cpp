#include "RenderSystem.h"

#include <GL/glew.h>

#include "SpriteRenderer.h"
#include "RawImage.h"

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
}


RenderSystem::~RenderSystem() {
	glUseProgram(0);

	for (Texture* tex : m_loadedTextures) {
		delete tex;
	}

	delete m_spriteRenderer;
}

Texture* RenderSystem::createTexture2D(const RawImage& img, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT) {
	Texture* tex = new Texture(Texture::Target::TEXTURE_2D, GL_TEXTURE0);

	tex->bind();
	glTexImage2D(tex->m_target, 0, GL_RGBA, img.getWidth(), img.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getData());

	glTexParameteri(tex->m_target, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(tex->m_target, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(tex->m_target, GL_TEXTURE_WRAP_S, textureWrapS);
	glTexParameteri(tex->m_target, GL_TEXTURE_WRAP_T, textureWrapT);

	tex->unbind();

	m_loadedTextures.push_back(tex);

	return tex;
}

Texture* RenderSystem::createTexture2DArray(RawImage* imgs, uint32 layers, Texture::Filter filtering, Texture::Wrap textureWrapS, Texture::Wrap textureWrapT) {

	uint32 width = imgs[0].getWidth();
	uint32 height = imgs[0].getHeight();
	uint32 subImgSize = width * height * imgs[0].getChannels();

	uint8* data = new uint8[subImgSize * layers];

	// TODO Ensure all images are the same size
	for (uint32 i = 0; i < layers; i++) {
		uint8* subImgData = imgs[layers].getData();
		for (uint32 j = 0; j < subImgSize; j++) {
			data[(i * subImgSize) + j] = subImgData[j];
		}
	}

	Texture* tex = new Texture(Texture::Target::TEXTURE_2D_ARRAY, GL_TEXTURE0);

	tex->bind();
	glTexImage3D(tex->m_target, 0, GL_RGBA, width, height, layers, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(tex->m_target, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(tex->m_target, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(tex->m_target, GL_TEXTURE_WRAP_S, textureWrapS);
	glTexParameteri(tex->m_target, GL_TEXTURE_WRAP_T, textureWrapT);
	tex->unbind();

	m_loadedTextures.push_back(tex);

	return tex;
}
