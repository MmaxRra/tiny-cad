#pragma once

#include <glad/glad.h>
#include "../core/frameTimer.h"
#include "../render/shader.h"

class Window;

class App {

public:

	explicit App(Window& window);
	~App();

	App(const App&) = delete;
	App& operator=(const App&) = delete;

	void run();

private:

	void update(float dt);
	void render(int width, int height);
	void everySecond();

	Window&    m_window;
	FrameTimer m_timer;

	ShaderProgram m_testShader { "assets/shaders/test" };
	GLuint        m_emptyVao = 0;
};