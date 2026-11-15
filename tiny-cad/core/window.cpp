
#include "window.h"

#include <glad/glad.h>
#include "../debug/gl_debug.h"

Window::Window(const WindowConfig& config) {

	// start from default so previous window hints dont leak in
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, config.debugContext ? GLFW_TRUE : GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, config.samples);

	m_window = glfwCreateWindow(config.width, config.height, config.title.c_str(), nullptr, nullptr);
		
	if (!m_window) {
		std::printf("[window] failed to create window with a GL 4.6 core context\n");
		return;
	}

	glfwMakeContextCurrent(m_window);
	
	static bool gladLoaded = false;
	
	if (!gladLoaded) {

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			
			std::printf("[window] failed to load OpenGL functions (glad)\n");
			glfwDestroyWindow(m_window);
			
			m_window = nullptr;
			return;
		}

		gladLoaded = true;
	}

	if (config.debugContext) enableGLDebugOutput();
	if (config.samples > 0) glEnable(GL_MULTISAMPLE);
	setVsync(config.vsync);
}

Window::~Window() { if (m_window) glfwDestroyWindow(m_window); }
bool Window::shouldClose() const { return glfwWindowShouldClose(m_window); }

void Window::pollEvents()  { glfwPollEvents(); }
void Window::waitEvents()  { glfwWaitEvents(); }
void Window::swapBuffers() { glfwSwapBuffers(m_window); }

FramebufferSize Window::framebufferSize() const {

	FramebufferSize s;
	glfwGetFramebufferSize(m_window, &s.w, &s.h);
	return s;
}

void Window::setTitle(const std::string& title) 
{ glfwSetWindowTitle(m_window, title.c_str()); }

void Window::setVsync(bool enabled) {

	m_vsync = enabled;
	glfwSwapInterval(enabled ? 1 : 0);
}