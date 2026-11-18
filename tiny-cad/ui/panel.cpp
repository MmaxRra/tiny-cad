
#include "panel.h"

Panel::Panel(float x, float y, float w, float h, glm::vec4 aColor) {

	m_pos   = glm::vec2(x, y);
	m_size  = glm::vec2(w, h);
	m_Color = aColor;
}

void Panel::addElement(glm::vec2 pos, glm::vec2 size, std::function<void()> onClick) {

	Element e;
	e.pos = pos;
	e.size = size;

	e.onClick = std::move(onClick);
	m_elements.push_back(std::move(e));
}

void Panel::clampTo(glm::vec2 screenSize) {

	glm::vec2 maxPos = glm::max(screenSize - m_size, glm::vec2(0.0f));
	m_pos = glm::clamp(m_pos, glm::vec2(0.0f), maxPos);
}

glm::vec2 Panel::getPosition() const { return m_pos; }
void Panel::setPosition(glm::vec2 pos) { m_pos = pos; }

bool Panel::contains(glm::vec2 p) const {

	return p.x >= m_pos.x && p.x < m_pos.x + m_size.x
		&& p.y >= m_pos.y && p.y < m_pos.y + m_size.y;
}

bool Panel::headerContains(glm::vec2 p) const {

	return p.x >= m_pos.x && p.x < m_pos.x + m_size.x
		&& p.y >= m_pos.y && p.y < m_pos.y + m_headerHeight;
}

int Panel::update(const UIInput& input, bool mouseOver) {

	glm::vec2 local = input.mouse - m_pos;

	m_hoveredElement = -1;
	if (mouseOver && !headerContains(input.mouse)) {

		for (int i = 0; i < (int)m_elements.size(); ++i) {

			if (m_elements[i].contains(local)) { m_hoveredElement = i; break; }
		}
	}

	if (input.pressed && m_hoveredElement != -1) m_pressedElement = m_hoveredElement;

	int clicked = -1;
	if (input.released) {

		if (m_pressedElement != -1 && m_pressedElement == m_hoveredElement) clicked = m_pressedElement;
		m_pressedElement = -1;
	}

	return clicked;
}

void Panel::draw(Batch2d& batch) const {

	batch.rect(m_pos.x, m_pos.y, m_size.x, m_size.y, m_Color);
	
	glm::vec4 headerColor = m_Color + glm::vec4(0.05f, 0.05f, 0.06f, 0.0f);
	batch.rect(m_pos.x, m_pos.y, m_size.x, m_headerHeight, headerColor);

	for (int i = 0; i < (int)m_elements.size(); ++i) {

		const Element& e = m_elements[i];

		glm::vec4 c = e.color;
		if (i == m_pressedElement && i == m_hoveredElement) c = e.pressedColor;
		else if (i == m_hoveredElement) c = e.hoverColor;

		batch.rect(m_pos.x + e.pos.x, m_pos.y + e.pos.y, e.size.x, e.size.y, c);
	}
}