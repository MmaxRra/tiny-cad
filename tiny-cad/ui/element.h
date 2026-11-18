#pragma once

#include <glm/glm.hpp>
#include <functional>
#include <string>

struct Element {
	glm::vec2 pos{ 0.0f };
	glm::vec2 size{ 0.0f };

	glm::vec4 color{ 0.30f, 0.31f, 0.35f, 1.0f };
	glm::vec4 hoverColor{ 0.36f, 0.38f, 0.43f, 1.0f };
	glm::vec4 pressedColor{ 0.25f, 0.26f, 0.30f, 1.0f };

	std::function<void()> onClick;   
	std::string label;

	bool contains(glm::vec2 p) const {

		return p.x >= pos.x && p.x < pos.x + size.x
			&& p.y >= pos.y && p.y < pos.y + size.y;
	}
};