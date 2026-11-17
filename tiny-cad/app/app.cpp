
#include "app.h"
#include "../core/window.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

App::App(Window& window) 
	: m_window(window), 
	  m_input(window), 
	  m_axes{ 2.0f }, 
	  m_overlay(),
	  m_partMesh(makeBox(BoxSize)),
	  m_partModel(glm::translate(glm::mat4(1.0f), BoxSize * 0.5f + glm::vec3(0.0f, 0.0f, -BoxSize.z)))
{
	glEnable(GL_DEPTH_TEST);

	m_panels.addPanel(20.0f, 60.0f, 280.0f, 400.0f, { 0.21f, 0.22f, 0.25f, 0.95f });
	m_panels.addPanel(340.0f, 120.0f, 240.0f, 180.0f, { 0.21f, 0.22f, 0.25f, 0.95f });
}

void App::run() {

	while (!m_window.shouldClose()) {

		m_timer.frameTick();
		m_input.beginFrame();
		m_window.pollEvents();

		FramebufferSize fb = m_window.framebufferSize();

		if (fb.isZero()) {
			m_window.waitEvents();
			continue;
		}

		if (m_timer.secondTicked()) everySecond();

		update(m_timer.dt());
		render(fb.w, fb.h);

		m_window.swapBuffers();
	}
}

void App::update(float dt) {

	glm::vec2 md = m_input.mouseDelta();
	if (m_input.mouseDown(GLFW_MOUSE_BUTTON_MIDDLE)) {

		if (m_input.shift()) {

			int winW = 0, winH = 0;
			glfwGetWindowSize(m_window.handle(), &winW, &winH);
			m_camera.pan(md.x, md.y, (float)winH);
		}
		else {

			const float sensitivity = 0.3f;
			m_camera.orbit(-md.x * sensitivity, md.y * sensitivity);
		}
	}

	float scroll = m_input.scrollDelta().y;
	if (scroll != 0.0f) m_camera.zoom(std::pow(0.9f, scroll));

	if (m_input.keyPressed(GLFW_KEY_HOME) || m_input.keyPressed(GLFW_KEY_KP_7)) m_camera.resetView();
	if (m_input.keyPressed(GLFW_KEY_F))										    m_camera.frameBounds({ -1, -1, -1 }, { 1, 1, 1 });
	if (m_input.keyPressed(GLFW_KEY_O))										    m_camera.setOrthographic(!m_camera.isOrthographic());
}

void App::render(int width, int height) {

	glViewport(0, 0, width, height);
	glClearColor(0.15f, 0.16f, 0.18f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_camera.setAspect((float)width / (float)height);

	m_grid.draw(m_camera);
	m_part.draw(m_camera, m_partMesh, m_partModel, glm::vec3(0.70f, 0.72f, 0.75f));
	
	int winW = 0, winH = 0;
	glfwGetWindowSize(m_window.handle(), &winW, &winH);
	
	m_overlay.begin((float)winW, (float)winH);
	m_panels.draw(m_overlay);
	m_overlay.drawFrame();
}

void App::everySecond() {

	char title[128];
	std::snprintf(title, sizeof(title), "tiny-cad | %d fps (%.2f ms)", m_timer.fps(), m_timer.frameMs());

	m_window.setTitle(title);
}