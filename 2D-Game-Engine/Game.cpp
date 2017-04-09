#include "Game.h"

#include "RenderSystem.h"
#include <GLFW/glfw3.h>
#include "Window.h"

#include "Rectangle.h"
#include "Camera.h"

#include "Input.h"

Game::Game(string title, int32 width, int32 height, const Rectangle& viewPort) : m_shutdown(false), m_fps(0) {
	DEBUG_LOG("Initializing game...");

	DEBUG_LOG("Initializing window...");
	m_window = new Window(title, width, height);

	DEBUG_LOG("Initializing render system...");
	m_renderSystem = new RenderSystem(Camera(viewPort));
}


Game::~Game() {
	DEBUG_LOG("Destroying game");
	delete m_renderSystem;
	delete m_window;
}

void Game::run() {
	init();
	int32 frames = 0;
	float64 counter = 0;
	float64 delta = 0;
	float64 currentTime = 0;

	DEBUG_LOG("Entering loop");

	while (!m_window->shouldClose() && !m_shutdown) {


		Window::pollEvents();

		currentTime = glfwGetTime();

		tick((float)delta);

		render();


		m_window->swapBuffers();
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

}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	m_renderSystem->getCamera().updateViewProjectionMatrix();
}