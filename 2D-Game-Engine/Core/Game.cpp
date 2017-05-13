#include "Game.h"

#include "Graphics/RenderSystem.h"
#include "Graphics/Camera.h"
#include "Graphics/Window.h"

#include "Math/Geometry/Rectangle.h"

#include "Input/Input.h"

#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Physics/Collision/CollisionSystem.h"

#include <GLFW/glfw3.h>

#ifdef DEBUG_BUILD
#include "Core/Debug.h"
#endif

Game::Game(const string& title, int32 width, int32 height, const Rectangle& viewPort) : m_shutdown(false), m_fps(0) {
	DEBUG_LOG("Initializing game...");

	DEBUG_LOG("Initializing window...");
	Window::init(title, width, height);
	Input::init();

	DEBUG_LOG("Initializing render system...");
	RenderSystem::init(Camera(viewPort));

	DEBUG_LOG("Initializing scene manager...");
	SceneManager::init();

#ifdef DEBUG_BUILD
	m_debug.reset(new Debug(this));
#endif
}


Game::~Game() {
	DEBUG_LOG("Destroying game");
	SceneManager::destroy();
	RenderSystem::destroy();
	Input::destroy();
	Window::destroy();
}

void Game::run() {
	
	init();

	int32 frames = 0;
	float64 counter = 0;
	float64 delta = 0;
	float64 currentTime = 0;

	DEBUG_LOG("Entering loop");

	while (!Window::get()->shouldClose() && !m_shutdown) {


		Window::pollEvents();

		currentTime = glfwGetTime();

		tick((float)delta);

		render();


		Window::get()->swapBuffers();
		delta = glfwGetTime() - currentTime;

		counter += delta;
		if (counter >= 1) {
			counter = 0;
			m_fps = frames;
			frames = 0;
		}
		else {
			frames++;
		}
	}
	DEBUG_LOG("Shutting down");
}

void Game::tick(float32 delta) {
	SceneManager::get()->tickCurrentScene(delta, this);
	SceneManager::get()->getCurrentScene().getCollisionSystem()->narrowScan();
#ifdef DEBUG_BUILD
	m_debug->tick();
#endif
}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	RenderSystem::get()->getCamera().updateViewProjectionMatrix();
	SceneManager::get()->renderCurrentScene();

#ifdef DEBUG_BUILD
	m_debug->render();
#endif
}