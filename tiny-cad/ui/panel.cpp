
#include "panel.h"
#include "../render/font.h"

Panel::Panel(float x, float y, float w, float h, glm::vec4 aColor, std::string title) {

	m_pos   = glm::vec2(x, y);
	m_size  = glm::vec2(w, h);
	m_title = title;
	m_color = aColor;
}

void Panel::addElement(glm::vec2 pos, glm::vec2 size, const std::string& label, std::function<void()> onClick) {

	Element e;

	e.pos = pos;
	e.size = size;
	e.label = label;

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
		&& p.y >= m_pos.y && p.y < m_pos.y + HeaderHeight;
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

void Panel::setIcon(int element, const Icon& icon) { m_elements[element].icon = icon; }

void Panel::draw(Batch2d& batch, const Font& font) const {

	const glm::vec4 textColor{ 0.85f, 0.86f, 0.90f, 1.0f };

	batch.rect(m_pos.x, m_pos.y, m_size.x, m_size.y, m_color);
	
	glm::vec4 headerColor = m_color + glm::vec4(0.05f, 0.05f, 0.06f, 0.0f);
	batch.rect(m_pos.x, m_pos.y, m_size.x, HeaderHeight, headerColor);

	float titleBaseline = m_pos.y + (HeaderHeight + font.size() * 0.7f) * 0.5f;
	font.drawText(batch, m_pos.x + 10.0f, titleBaseline, m_title, textColor);

	for (int i = 0; i < (int)m_elements.size(); ++i) {
		
		const Element& e = m_elements[i];
		glm::vec4 c = e.color;
		glm::vec4 ic = e.iconColor;

		if (i == m_pressedElement && i == m_hoveredElement) c = e.pressedColor;
		else if (i == m_hoveredElement)                     c = e.hoverColor;

		if (i == m_pressedElement && i == m_hoveredElement) { c = e.pressedColor; ic = e.iconPressedColor; }
		else if (i == m_hoveredElement) { c = e.hoverColor;   ic = e.iconHoverColor; }

		float ex = m_pos.x + e.pos.x;
		float ey = m_pos.y + e.pos.y;

		batch.rect(ex, ey, e.size.x, e.size.y, c);

		if (!e.label.empty()) {

			float tx = ex + (e.size.x - font.textWidth(e.label)) * 0.5f;
			float ty = ey + (e.size.y + font.size() * 0.7f) * 0.5f;
			font.drawText(batch, tx, ty, e.label, textColor);
		}

		if (e.icon.texture) {
			float ix, iy = ey + (e.size.y - e.icon.size.y) * 0.5f;			  // vertically centered
			if (e.label.empty()) ix = ex + (e.size.x - e.icon.size.x) * 0.5f; // icon only: centered
			else                 ix = ex + 8.0f;                              // with label: on the left

			batch.texturedRect(ix, iy, e.icon.size.x, e.icon.size.y, e.icon.texture, e.icon.uvMin, e.icon.uvMax, e.iconColor);
		}
	}
}