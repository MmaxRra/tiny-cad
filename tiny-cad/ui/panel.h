#pragma once

#include <glm/glm.hpp>
#include <functional>
#include <vector>

#include "../render/batch2d.h"
#include "uiInput.h"
#include "element.h"

class Panel {

public:

	Panel(float x, float y, float w, float h, glm::vec4 aColor);
	~Panel() = default;

	void addElement(glm::vec2 pos, glm::vec2 size, std::function<void()> onClick);
	void clampTo(glm::vec2 screenSize);

	glm::vec2 getPosition() const;
	void setPosition(glm::vec2 pos);
	
	bool contains(glm::vec2 p) const;
	bool headerContains(glm::vec2 p) const;

	int update(const UIInput& input, bool mouseOver);
	void draw(Batch2d& batch) const;

	const std::function<void()>& getOnClick(int element) const { return m_elements[element].onClick; }

private:

	static constexpr float m_headerHeight = 28.0f;

	glm::vec2 m_pos;
	glm::vec2 m_size;
	glm::vec4 m_Color;

	std::vector<Element> m_elements;
	int m_hoveredElement = -1;
	int m_pressedElement = -1;
};