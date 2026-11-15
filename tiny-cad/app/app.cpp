
#include "app.h"
#include "../core/window.h"
#include <cstdio>

App::App(Window& window) : m_window(window), m_input(window) { glCreateVertexArrays(1, &m_emptyVao); }
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

	if (m_input.keyPressed(GLFW_KEY_F2)) {
		m_window.setVsync(!m_window.vsync());
		std::printf("vsync %s\n", m_window.vsync() ? "on" : "off");
	}

	if (m_input.ctrl() && m_input.keyPressed(GLFW_KEY_Z)) std::printf("ctrl+z\n");

	if (m_input.mouseDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
		glm::vec2 d = m_input.mouseDelta();
		if (d.x != 0.0f || d.y != 0.0f) std::printf("drag %.1f %.1f\n", d.x, d.y);
	}

	glm::vec2 s = m_input.scrollDelta();
	if (s.y != 0.0f) std::printf("scroll %.1f\n", s.y);

	for (unsigned int c : m_input.typedChars()) std::printf("char U+%04X\n", c);
	if (m_input.mouseReleased(GLFW_MOUSE_BUTTON_LEFT)) std::printf("left released\n");
}

void App::render(int width, int height) {

	glViewport(0, 0, width, height);
	glClearColor(0.15f, 0.16f, 0.18f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_testShader.setFloat("uTime", (float)m_timer.time());
	m_testShader.use();

	glBindVertexArray(m_emptyVao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void App::everySecond() {

	m_testShader.reloadIfChanged();

	char title[128];
	std::snprintf(title, sizeof(title), "tiny-cad | %d fps (%.2f ms)",
		m_timer.fps(), m_timer.frameMs());

	m_window.setTitle(title);
}