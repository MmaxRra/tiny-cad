
#include "app.h"
#include "../core/window.h"
#include <cstdio>

App::App(Window& window) : m_window(window) { glCreateVertexArrays(1, &m_emptyVao); }
App::~App() { glDeleteVertexArrays(1, &m_emptyVao); }

void App::run() {

	while (!m_window.shouldClose()) {

		m_timer.frameTick();
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

	// input, camera, tools .. all requiring dt
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