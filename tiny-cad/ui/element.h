#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <functional>
#include <string>

struct Icon {
	GLuint    texture = 0;                 // 0 = no icon
	glm::vec2 uvMin{ 0.0f, 0.0f };         // region of the texture (whole image by default)
	glm::vec2 uvMax{ 1.0f, 1.0f };
	glm::vec2 size{ 0.0f, 0.0f };		   // drawn size in pixels
};

struct Element {
	glm::vec2 pos{ 0.0f };
	glm::vec2 size{ 0.0f };

	std::function<void()> onClick;   
	std::string label;

	Icon icon;
	bool contains(glm::vec2 p) const {

		return p.x >= pos.x && p.x < pos.x + size.x
			&& p.y >= pos.y && p.y < pos.y + size.y;
	}
};