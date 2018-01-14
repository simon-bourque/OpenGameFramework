#include "Game3D.h"

#include "Core/Singleton.h"
#include "Core/Platform.h"

#include "Core/Console/Console.h"

#include "Core/Window/Window.h"

#include "Core/Input/Input.h"

#include "3D/Scene/SceneManager3D.h"

#include "Core/Graphics/GraphicsContext.h"

#include <GL/glew.h>

Game3D::Game3D(const string& title, int32 width, int32 height) {
	Singleton<Console>::init();

	DEBUG_LOG("Initializing game...");
	printSystemInfo();

	DEBUG_LOG("Initializing window...");
	Singleton<Window>::init(title, width, height);
	Singleton<Input>::init();

	DEBUG_LOG("Initializing graphics...");
	Singleton<GraphicsContext>::init();

	DEBUG_LOG("Initializing scene manager...");
	Singleton<SceneManager3D>::init();
	//
	//	//DEBUG_LOG("Initializing sound engine...");
	//	//SoundEngine::init();
	//
#ifdef DEBUG_BUILD
	Singleton<Debug>::init();
#endif
}

Game3D::~Game3D() {
	DEBUG_LOG("Destroying game");
#ifdef DEBUG_BUILD
	Singleton<Debug>::destroy();
#endif
	//	//SoundEngine::destroy();
	Singleton<GraphicsContext>::destroy();
	Singleton<Input>::destroy();
	Singleton<Window>::destroy();
	Singleton<Console>::destroy();
}

void Game3D::tick(float32 deltaSeconds) {
	getSceneManager3DInstance()->tickCurrentScene(deltaSeconds);
	getSceneManager3DInstance()->getCurrentScene().getCollisionSystem()->narrowScan();
#ifdef DEBUG_BUILD
	getDebugInstance()->tick(m_fps);
#endif
}

void Game3D::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	getGraphics3DInstance()->getCamera().updateViewProjectionMatrix();
	getSceneManager3DInstance()->renderCurrentScene();

#ifdef DEBUG_BUILD
	getDebugInstance()->render();
#endif
}