#pragma once

#include <glm/glm.hpp>
#include <functional>
#include <vector>
#include <string>

#include "../render/batch2d.h"
#include "uiInput.h"
#include "element.h"
#include "theme.h"

class Font;
class Panel {

public:

	Panel(float x, float y, float w, float h, const std::string& title);
	~Panel() = default;

	void addElement(glm::vec2 pos, glm::vec2 size, const std::string& label, std::function<void()> onClick);
	void clampTo(glm::vec2 screenSize);

	glm::vec2 getPosition() const;
	void setPosition(glm::vec2 pos);
	
	bool contains(glm::vec2 p) const;
	bool headerContains(glm::vec2 p, const Theme& theme) const;

	int update(const UIInput& input, bool mouseOver, const Theme& theme);
	void draw(Batch2d& batch, const Font& font, const Theme& theme) const;

	void setIcon(int element, const Icon& icon);
	const std::function<void()>& getOnClick(int element) const { return m_elements[element].onClick; }

private:

	std::string m_title;

	glm::vec2 m_pos;
	glm::vec2 m_size;

	std::vector<Element> m_elements;
	int m_hoveredElement = -1;
	int m_pressedElement = -1;
};