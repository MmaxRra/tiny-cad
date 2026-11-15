
#include "input.h"
#include "window.h"

namespace {
	bool validKey(int key) { return key >= 0 && key <= GLFW_KEY_LAST; }
	bool validButton(int button) { return button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST; }
}

Input::Input(Window& window) : m_window(window.handle()) {

	glfwSetWindowUserPointer(m_window, this);

	// hook glfw callbacks
	glfwSetKeyCallback(m_window, keyCallback);
	glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
	glfwSetCursorPosCallback(m_window, cursorPosCallback);
	glfwSetScrollCallback(m_window, scrollCallback);
	glfwSetCharCallback(m_window, charCallback);
	glfwSetWindowFocusCallback(m_window, focusCallback);

	// start values of cursor
	double x = 0.0, y = 0.0;
	glfwGetCursorPos(m_window, &x, &y);
	m_mousePos    = { (float)x, (float)y };
	m_hasMousePos = true;
}

Input::~Input()
{
	// unhook so GLFW never calls into a destroyed object
	glfwSetKeyCallback(m_window, nullptr);
	glfwSetMouseButtonCallback(m_window, nullptr);
	glfwSetCursorPosCallback(m_window, nullptr);
	glfwSetScrollCallback(m_window, nullptr);
	glfwSetCharCallback(m_window, nullptr);
	glfwSetWindowFocusCallback(m_window, nullptr);
	glfwSetWindowUserPointer(m_window, nullptr);
}

void Input::beginFrame() {

	m_prevKeys    = m_keys;
	m_prevButtons = m_buttons;

	m_repeated.fill(false);
	m_mouseDelta = { 0.0f, 0.0f };
	m_scroll     = { 0.0f, 0.0f };

	m_chars.clear();
}

void Input::clearHeld() {

	m_keys.fill(false);
	m_buttons.fill(false);
}

// queries
bool Input::keyDown(int key) const { return validKey(key) && m_keys[key]; }
bool Input::keyPressed(int key) const { return validKey(key) && m_keys[key] && !m_prevKeys[key]; }
bool Input::keyReleased(int key) const { return validKey(key) && !m_keys[key] && m_prevKeys[key]; }
bool Input::keyRepeated(int key) const { return validKey(key) && m_repeated[key]; }

bool Input::mouseDown(int b) const { return validButton(b) && m_buttons[b]; }
bool Input::mousePressed(int b) const { return validButton(b) && m_buttons[b] && !m_prevButtons[b]; }
bool Input::mouseReleased(int b) const { return validButton(b) && !m_buttons[b] && m_prevButtons[b]; }

bool Input::shift() const { return keyDown(GLFW_KEY_LEFT_SHIFT) || keyDown(GLFW_KEY_RIGHT_SHIFT); }
bool Input::ctrl() const { return keyDown(GLFW_KEY_LEFT_CONTROL) || keyDown(GLFW_KEY_RIGHT_CONTROL); }
bool Input::alt() const { return keyDown(GLFW_KEY_LEFT_ALT) || keyDown(GLFW_KEY_RIGHT_ALT); }

// glfw callbacks
Input* Input::from(GLFWwindow* window) 
{ return static_cast<Input*>(glfwGetWindowUserPointer(window)); }

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input* self = from(window);
	if (!self || !validKey(key)) return; // GLFW_KEY_UNKNOWN is -1

	if (action == GLFW_PRESS)        self->m_keys[key] = true;
	else if (action == GLFW_RELEASE) self->m_keys[key] = false;
	else if (action == GLFW_REPEAT)  self->m_repeated[key] = true;
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Input* self = from(window);
	if (!self || !validButton(button)) return;

	self->m_buttons[button] = (action == GLFW_PRESS);
}

void Input::cursorPosCallback(GLFWwindow* window, double x, double y)
{
	Input* self = from(window);
	if (!self) return;

	glm::vec2 pos{ (float)x, (float)y };
	if (self->m_hasMousePos) self->m_mouseDelta += pos - self->m_mousePos; 

	self->m_mousePos = pos;
	self->m_hasMousePos = true;
}

void Input::scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	Input* self = from(window);
	if (!self) return;

	self->m_scroll += glm::vec2{ (float)xOffset, (float)yOffset };
}

void Input::charCallback(GLFWwindow* window, unsigned int codepoint)
{
	Input* self = from(window);
	if (!self) return;

	self->m_chars.push_back(codepoint);
}

void Input::focusCallback(GLFWwindow* window, int focused)
{
	Input* self = from(window);
	if (!self) return;

	// releases never arrive while unfocused
	if (!focused) self->clearHeld(); 
}