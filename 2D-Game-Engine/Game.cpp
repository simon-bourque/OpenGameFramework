#include "Game.h"

#include <GLFW/glfw3.h>

#include "Window.h"

Game::Game(string title, int32 width, int32 height) : m_shutdown(false), m_fps(0) {
	DEBUG_LOG("Initializing game.");

	DEBUG_LOG("Initializing window.");
	m_window = new Window(title, width, height);
}


Game::~Game() {
	DEBUG_LOG("Destroying game.");
	delete m_window;
}

void Game::run() {
	init();
	int32 frames = 0;
	float64 counter = 0;
	float64 delta = 0;
	float64 currentTime = 0;

	DEBUG_LOG("Entering loop.");

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
}

void Game::tick(float32 delta) {

}

void Game::render() {

}