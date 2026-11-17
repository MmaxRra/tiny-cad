#pragma once

#include <glm/glm.hpp>
#include "../render/batch2d.h"

class Panel {

public:

	Panel(float x, float y, float w, float h, glm::vec4 aColor);
	~Panel() = default;

	void draw(Batch2d& batch) const;

private:

	const float m_headerHeight = 28.0f;

	glm::vec2 m_pos;
	glm::vec2 m_size;
	glm::vec4 m_Color;
};