#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <array>
#include <vector>


class Window;
class Input {

public:

	explicit Input(Window& window);
	~Input();

	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(Input&&) = delete;

	// copies frames key and button state into previous
	// and resets per-frame values (mouse delta, scroll, typed chars)
	void beginFrame();

	// keyboard functions
	bool keyDown(int key) const;
	bool keyPressed(int key) const;
	bool keyReleased(int key) const; 
	bool keyRepeated(int key) const;

	// mouse functions
	bool mouseDown(int button) const;
	bool mousePressed(int button) const;
	bool mouseReleased(int button) const;

	glm::vec2 mousePosition() const { return m_mousePos; }
	glm::vec2 mouseDelta()    const { return m_mouseDelta; }
	glm::vec2 scrollDelta()   const { return m_scroll; }

	// modifier functions
	bool shift() const;
	bool ctrl() const;
	bool alt() const;

	// typed charachters
	const std::vector<unsigned int>& typedChars() const { return m_chars; }

private:

	static constexpr int KeyCount    = GLFW_KEY_LAST + 1;
	static constexpr int ButtonCount = GLFW_MOUSE_BUTTON_LAST + 1;

	GLFWwindow* m_window = nullptr;

	std::array<bool, KeyCount>    m_keys{};
	std::array<bool, KeyCount>    m_prevKeys{};
	std::array<bool, KeyCount>    m_repeated{};
	std::array<bool, ButtonCount> m_buttons{};
	std::array<bool, ButtonCount> m_prevButtons{};

	glm::vec2 m_mousePos{ 0.0f };
	glm::vec2 m_mouseDelta{ 0.0f };
	glm::vec2 m_scroll{ 0.0f };
	bool	  m_hasMousePos = false;

	std::vector<unsigned int> m_chars;
	void clearHeld();

	static Input* from(GLFWwindow* window);

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double x, double y);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void charCallback(GLFWwindow* window, unsigned int codepoint);
	static void focusCallback(GLFWwindow* window, int focused);
};