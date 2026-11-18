#pragma once

#include <glm/glm.hpp>

struct UIInput {
	glm::vec2 mouse{ 0.0f };
	glm::vec2 screenSize{ 0.0f };

	bool pressed = false;
	bool held = false;
	bool released = false;
};