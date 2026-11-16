#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

struct WindowConfig {
	int width		  = 1280;
	int height		  = 720;
	std::string title = "tiny-cad";
	int samples		  = 4;		    // MSAA samples, 0 = off
	bool vsync		  = true;		
	bool debugContext = true;		// gl debug output
};

struct FramebufferSize {
	int w = 0, h = 0;

	bool isZero() const { return w == 0 || h == 0; }
	float aspect() const { return h > 0 ? (float)w / (float)h : 1.0f; }
};

// One OS window with its own OpenGL core context.
// Requires a live Platform (glfwInit) for its whole lifetime.

class Window {

public:

	explicit Window(const WindowConfig& config = {});
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;

	bool isValid() const { return m_window != nullptr; }

	bool shouldClose() const;
	void pollEvents();
	void waitEvents();
	void swapBuffers();

	FramebufferSize framebufferSize() const;

	void setTitle(const std::string& title);
	void setVsync(bool enabled);
	bool vsync() const { return m_vsync; }

	GLFWwindow* handle() const { return m_window; }

private:

	GLFWwindow* m_window = nullptr;
	bool		m_vsync  = true;
};