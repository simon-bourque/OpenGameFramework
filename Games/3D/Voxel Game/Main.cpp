// Standard library
#include <iostream>
#include <vector>

// External includes
#include <GL/glew.h>
#include <glm/glm.hpp>

// OGF Core
#include "Core/Game.h"
#include "Core/Types.h"
#include "Core/Time.h"
#include "Core/Input/Input.h"
#include "Core/Console/Console.h"
#include "Core/Window/Window.h"
#include "Core/Graphics/TextureCache.h"
#include "Core/Graphics/Texture.h"

// OGF 3D
#include "3D/Graphics/Camera.h"
#include "3D/Graphics/FreeCameraController.h"
#include "3D/Graphics/WaterRenderer.h"
#include "3D/Graphics/Meshes/Model.h"
#include "3D/Graphics/Meshes/ModelCache.h"
#include "3D/Graphics/Shader/ShaderCache.h"
#include "3D/Graphics/Lighting/LightSource.h"
#include "3D/Graphics/Renderer/RenderingContext.h"

//VoxelGame
#include "Graphics/ShadowMap.h"
#include "Chunking/ChunkManager.h"

void update(float32 deltaSeconds);
void render();

// Test cube, will be removed later
#include "Chunking/Primitives.h"
void initTestCube();
ShaderProgramOld* cubeShader = nullptr;
Model* cubeModel = nullptr;
Texture* cubeTexture = nullptr;

int32 SCREENWIDTH = 600, SCREENHEIGHT = 480;
glm::vec3 playerPosition(0, 160, 0);
LightSource* sun = nullptr;
ShadowMap* shadowMap = nullptr;
glm::vec3 lightDirection(0.0f, -0.2f, -0.5f);

ShaderProgramOld* chunkShader = nullptr;
Texture* chunkTexture = nullptr;

void initSkybox();
unsigned int skyboxVAO, skyboxVBO;
ShaderProgramOld* skyboxShader = nullptr;
Model* skyboxModel = nullptr;
Texture* skyboxTexture = nullptr;

Window* gWindow = nullptr;

glm::vec3 camPositionVector;
FreeCameraController* gCameraController;

//#define COMPILE_DRAW_NORMALS // Uncomment me if you want to render normals

#ifdef COMPILE_DRAW_NORMALS
ShaderProgram* chunkNormalsShader = nullptr;
#endif

#define RENDER_WATER	// Comment me if you don't want to render water
#define RENDER_SHADOWS	// Comment me if you don't want to render shadows

int main() {

	try {
		
		// Initialize Window and Input
		Singleton<Console>::init();
		Singleton<Window>::init("Voxel Game", SCREENWIDTH, SCREENHEIGHT);
		Singleton<Input>::init();

		RenderingContext::init();
		RenderingContext::get()->camera.setPerspective(45, SCREENWIDTH/(float32)SCREENHEIGHT);
		chunkShader = RenderingContext::get()->shaderCache.loadShaderProgram("chunk_shader", "Resources/Shaders/chunk_vert.glsl", "Resources/Shaders/chunk_frag.glsl");
		chunkTexture = RenderingContext::get()->textureCache.loadTexture2DArray("chunk_texture", 7, "Resources/Images/tiles.png");

		// Load skybox texture
		CubeMapPaths paths;
		paths.bk = "Resources/Images/MCLITEbk.tga";
		paths.ft = "Resources/Images/MCLITEft.tga";
		paths.up = "Resources/Images/MCLITEup.tga";
		paths.dn = "Resources/Images/MCLITEdn.tga";
		paths.lf = "Resources/Images/MCLITElf.tga";
		paths.rt = "Resources/Images/MCLITErt.tga";
		skyboxShader = RenderingContext::get()->shaderCache.loadShaderProgram("skybox_shader", "Resources/Shaders/skybox_vert.glsl", "Resources/Shaders/skybox_frag.glsl");
		skyboxTexture = RenderingContext::get()->textureCache.loadTextureCubeMap("skybox_texture", paths);

		initSkybox();

#ifdef COMPILE_DRAW_NORMALS
		chunkNormalsShader = RenderingContext::get()->shaderCache.loadShaderProgram("chunk_shader", "chunk_vert.glsl", "chunk_normals_frag.glsl", "chunk_normals_geo.glsl");
		//glLineWidth(3.0f);
#endif
		initTestCube();

		WaterRenderer::init();
		WaterRenderer::get()->buildFBO(SCREENWIDTH, SCREENHEIGHT);
	}
	catch (std::runtime_error& ex) {
		std::cout << ex.what() << std::endl;
		system("pause");
		return 1;
	}

	// FreeCam
	gCameraController = new FreeCameraController(&RenderingContext::get()->camera);

	RenderingContext::get()->camera.transform.translateLocal(playerPosition.x, playerPosition.y, playerPosition.z);
	RenderingContext::get()->camera.transform.orient(glm::degrees(-0.0f), 0, 0);

	// Load face data for shader
	std::vector<glm::vec3> faceData;
	faceData.push_back(glm::vec3(1, 0, 2)); // Grass
	faceData.push_back(glm::vec3(3, 3, 3)); // Log
	faceData.push_back(glm::vec3(4, 4, 4)); // Leaves
	faceData.push_back(glm::vec3(2, 2, 2)); // Dirt
	faceData.push_back(glm::vec3(5, 5, 5)); // Sand
	faceData.push_back(glm::vec3(6, 6, 6)); // Snow
	chunkShader->use();
	chunkShader->setUniform("faceData", faceData);

	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

	sun = new LightSource(lightDirection, lightColor);
	shadowMap = new ShadowMap(SCREENWIDTH, SCREENHEIGHT, lightDirection);

	WaterRenderer::get()->setLightUniforms(*sun);

	initTestCube();

	// Start loop
	uint32 frames = 0;
	float64 counter = 0;
	float64 delta = 0;
	float64 currentTime = 0;

	while (!getWindowInstance()->shouldClose()) {

#ifdef DEBUG_BUILD
		BROFILER_FRAME("MainThread")
#endif
		getWindowInstance()->pollEvents();

		currentTime = getCurrentTime();

		update((float32)delta);
		render();
		
		getWindowInstance()->swapBuffers();

		delta = getCurrentTime() - currentTime;

		counter += delta;
		if (counter >= 1) {
			counter = 0;
			std::cout << "FPS: " << frames << std::endl;
			frames = 0;
		}
		else {
			frames++;
		}
	}

	delete gCameraController;
	delete shadowMap;
	delete sun;

	WaterRenderer::destroy();
	RenderingContext::destroy();

	return 0;
}

/*
glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int32 width, int32 height) -> void {
		
		// Width and height are 0 when we alt-tab while in fullscreen, I have no idea why it's done that way.
		if (width == 0 || height == 0) {
			return;
		}

		glViewport(0, 0, width, height);
		WaterRenderer::get()->resizeFBO(width, height);
		shadowMap->updateSize(width, height);
		SCREENWIDTH = width;
		SCREENHEIGHT = height;
		RenderingContext::get()->camera.setPerspective(45, SCREENWIDTH / (float32)SCREENHEIGHT);
});
*/

void update(float32 deltaSeconds) {
	// Update logic...
	gCameraController->update(deltaSeconds);

	static glm::vec3 position;

	position = glm::vec3(RenderingContext::get()->camera.transform.xPos,
		RenderingContext::get()->camera.transform.yPos,
		RenderingContext::get()->camera.transform.zPos);

	glm::vec3 currentChunk = ChunkManager::instance()->getCurrentChunk(position);

	// Spooky hack lol
	static glm::vec3 lastChunk(currentChunk.x + 1.0f, currentChunk.y, currentChunk.z);

	if (currentChunk != lastChunk) {
		ChunkManager::instance()->loadChunks(currentChunk);
		lastChunk = currentChunk;
	}
	ChunkManager::instance()->uploadQueuedChunk();

	chunkShader->use();
	chunkShader->setUniform("viewPos", position);

	static float32 panner = 0;
	panner += 0.05 * deltaSeconds;
	RenderingContext::get()->shaderCache.getShaderProgram("water_shader")->use();
	RenderingContext::get()->shaderCache.getShaderProgram("water_shader")->setUniform("viewPos", position);
	RenderingContext::get()->shaderCache.getShaderProgram("water_shader")->setUniform("panner", panner);
}

void render() {
	RenderingContext::get()->prepareFrame();

#ifdef COMPILE_DRAW_NORMALS
	chunkNormalsShader->use();
	chunkNormalsShader->setUniform("vpMatrix", RenderingContext::get()->camera.getViewProjectionMatrix());
#endif

	const std::unordered_map<int64, Chunk>& chunks = ChunkManager::instance()->getCurrentlyLoadedChunks();

	//First Pass (Shadows)
	RenderingContext::get()->shaderCache.getShaderProgram("sm_shader")->use();
	shadowMap->updateMVP(lightDirection);
	shadowMap->bindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);
#ifdef RENDER_SHADOWS
	for (const auto& chunk : chunks) {
		glBindVertexArray(chunk.second.getVao());
		glDrawElementsInstanced(GL_TRIANGLES, cube::numIndices, GL_UNSIGNED_INT, nullptr, chunk.second.getBlockCount());
	}
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	std::vector<const Chunk*> visibleChunks;

	/*
	// Cull chunks not visible to the camera
	for (const auto& chunk : chunks) {
		if (RenderingContext::get()->camera.intersectsFrustum(chunk.second)) {
			visibleChunks.push_back(&chunk.second);
		}
	}
	*/
	//std::cout << visibleChunks.size() << std::endl;

	// Second Pass (render refraction texture)
	glBindFramebuffer(GL_FRAMEBUFFER, WaterRenderer::get()->getRefractionFBO());
	chunkShader->use();
	chunkShader->setUniform("vpMatrix", RenderingContext::get()->camera.getViewProjectionMatrix());
	chunkShader->setUniform("waterPlaneHeight", WaterRenderer::get()->getY());
	chunkShader->setUniform("lightSpaceMatrix", shadowMap->getMVP());
	chunkTexture->bind(Texture::Unit::UNIT_0);
	shadowMap->bindForReading();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef RENDER_WATER
	glEnable(GL_CLIP_DISTANCE0);
	for (const auto& chunk : visibleChunks) {
		glBindVertexArray(chunk->getVao());
		glDrawElementsInstanced(GL_TRIANGLES, cube::numIndices, GL_UNSIGNED_INT, nullptr, chunk->getBlockCount());
	}
	glDisable(GL_CLIP_DISTANCE0);
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Render chunks
	for (const auto& chunk : visibleChunks) {
		glBindVertexArray(chunk->getVao());
		glDrawElementsInstanced(GL_TRIANGLES, cube::numIndices, GL_UNSIGNED_INT, nullptr, chunk->getBlockCount());

#ifdef COMPILE_DRAW_NORMALS
		chunkNormalsShader->use();
		glDrawElementsInstanced(GL_POINTS, cube::numIndices, GL_UNSIGNED_INT, nullptr, chunk->getBlockCount());
		chunkShader->use();
#endif
	}

	// Render water
#ifdef RENDER_WATER
	WaterRenderer::get()->prepare();
	for (const auto& chunk : visibleChunks) {
		glm::vec3 pos = chunk->getPosition();

		// Render water
		WaterRenderer::get()->render(pos.x, pos.z, ChunkManager::CHUNKWIDTH);
	}
#endif

	// Render test cube
	cubeShader->use();
	
	cubeModel->bind();
	
	cubeShader->setUniform("mvpMatrix", RenderingContext::get()->camera.getViewProjectionMatrix());
	cubeShader->setUniform("color", glm::vec3(1, 0, 0));
	
	glDrawElements(GL_TRIANGLES, cubeModel->getCount(), GL_UNSIGNED_INT, nullptr);

	// Render skybox
	glDepthFunc(GL_LEQUAL);
	skyboxShader->use();
	skyboxShader->setUniform("view", glm::mat4(glm::mat3(RenderingContext::get()->camera.getViewMatrix())));
	skyboxShader->setUniform("projection", RenderingContext::get()->camera.getProjectionMatrix());
	skyboxTexture->bind(Texture::Unit::UNIT_0);
	glBindVertexArray(skyboxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void initTestCube() {
	std::vector<float32> vertices;
	std::vector<float32> uvCoords;
	std::vector<float32> normals;
	std::vector<uint32> indices;

	cube::fill(vertices, uvCoords, normals, indices);

	cubeModel = RenderingContext::get()->modelCache.loadModel("cube", vertices, indices);
	cubeShader = RenderingContext::get()->shaderCache.loadShaderProgram("test_cube", "Resources/Shaders/test_cube_vert.glsl", "Resources/Shaders/test_cube_frag.glsl");
	cubeTexture = RenderingContext::get()->textureCache.loadTexture2D("test_cube_texture", "Resources/Images/test.png");
}

void initSkybox() {
	float skyboxVertices[] = {
		// positions          
		-512.0f,  512.0f, -512.0f,
		-512.0f, -512.0f, -512.0f,
		512.0f, -512.0f, -512.0f,
		512.0f, -512.0f, -512.0f,
		512.0f,  512.0f, -512.0f,
		-512.0f,  512.0f, -512.0f,

		-512.0f, -512.0f,  512.0f,
		-512.0f, -512.0f, -512.0f,
		-512.0f,  512.0f, -512.0f,
		-512.0f,  512.0f, -512.0f,
		-512.0f,  512.0f,  512.0f,
		-512.0f, -512.0f,  512.0f,

		512.0f, -512.0f, -512.0f,
		512.0f, -512.0f,  512.0f,
		512.0f,  512.0f,  512.0f,
		512.0f,  512.0f,  512.0f,
		512.0f,  512.0f, -512.0f,
		512.0f, -512.0f, -512.0f,

		-512.0f, -512.0f,  512.0f,
		-512.0f,  512.0f,  512.0f,
		512.0f,  512.0f,  512.0f,
		512.0f,  512.0f,  512.0f,
		512.0f, -512.0f,  512.0f,
		-512.0f, -512.0f,  512.0f,

		-512.0f,  512.0f, -512.0f,
		512.0f,  512.0f, -512.0f,
		512.0f,  512.0f,  512.0f,
		512.0f,  512.0f,  512.0f,
		-512.0f,  512.0f,  512.0f,
		-512.0f,  512.0f, -512.0f,

		-512.0f, -512.0f, -512.0f,
		-512.0f, -512.0f,  512.0f,
		512.0f, -512.0f, -512.0f,
		512.0f, -512.0f, -512.0f,
		-512.0f, -512.0f,  512.0f,
		512.0f, -512.0f,  512.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);\
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

glm::vec2 getMouseDelta() {
	// Poll mouse position
	float64 mPosX;
	float64 mPosY;
	getInputInstance()->getCursorPos(mPosX, mPosY);

	static float64 lastMousePosX = mPosX;
	static float64 lastMousePosY = mPosY;

	glm::vec2 mouseAxis(0, 0);

	if (mPosX != lastMousePosX || mPosY != lastMousePosY) {
		mouseAxis.x = mPosX - lastMousePosX;
		mouseAxis.y = lastMousePosY - mPosY;
	}

	lastMousePosX = mPosX;
	lastMousePosY = mPosY;

	return mouseAxis;
}
