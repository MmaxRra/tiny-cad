#pragma once

#include <glm/glm.hpp>
#include <functional>
#include <string>

struct Icon {
	GLuint    texture = 0;                 // 0 = no icon
	glm::vec2 uvMin{ 0.0f, 0.0f };         // region of the texture (whole image by default)
	glm::vec2 uvMax{ 1.0f, 1.0f };
	glm::vec2 size{ 20.0f, 20.0f };        // drawn size in pixels
};

struct Element {
	glm::vec2 pos{ 0.0f };
	glm::vec2 size{ 0.0f };

	glm::vec4 color{ 0.30f, 0.31f, 0.35f, 1.0f };
	glm::vec4 hoverColor{ 0.36f, 0.38f, 0.43f, 1.0f };
	glm::vec4 pressedColor{ 0.25f, 0.26f, 0.30f, 1.0f };

	glm::vec4 iconColor{ 0.80f, 0.81f, 0.85f, 1.0f };   
	glm::vec4 iconHoverColor{ 1.00f, 1.00f, 1.00f, 1.0f };   
	glm::vec4 iconPressedColor{ 0.65f, 0.66f, 0.70f, 1.0f };   

	std::function<void()> onClick;   
	std::string label;

	Icon icon;
	bool contains(glm::vec2 p) const {

		return p.x >= pos.x && p.x < pos.x + size.x
			&& p.y >= pos.y && p.y < pos.y + size.y;
	}
};