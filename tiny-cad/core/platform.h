#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstdio>

class Platform {

public:

	Platform() {

		glfwSetErrorCallback(errorCallback);
		m_valid = glfwInit() == GLFW_TRUE;

		if (!m_valid) std::printf("[platform] glfwInit failed \n");
	}

	~Platform() {

		if (m_valid) glfwTerminate();
	}

	Platform(const Platform&) = delete;
	Platform& operator=(const Platform&) = delete;
	Platform(Platform&&) = delete;
	Platform& operator=(Platform&&) = delete;

	bool isValid() const { return m_valid; }

private:

	bool m_valid = false;

	static void errorCallback(int code, const char* description) 
	{ std::printf("[glfw] error %d: %s\n", code, description); }
};