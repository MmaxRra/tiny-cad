
#include "app.h"
#include "../core/window.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

App::App(Window& window) : m_window(window), m_input(window), m_axes{ 2.0f } { glEnable(GL_DEPTH_TEST); glCreateVertexArrays(1, &m_emptyVao); }
App::~App() { glDeleteVertexArrays(1, &m_emptyVao); }

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

	//m_testShader.setFloat("uTime", (float)m_timer.time());
	//m_testShader.use();

	m_camera.setAspect((float)width / (float)height);
	m_axes.draw(m_camera.getViewProjection());

	glBindVertexArray(m_emptyVao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void App::everySecond() {

	m_testShader.reloadIfChanged();

	char title[128];
	std::snprintf(title, sizeof(title), "tiny-cad | %d fps (%.2f ms)", m_timer.fps(), m_timer.frameMs());

	m_window.setTitle(title);
}