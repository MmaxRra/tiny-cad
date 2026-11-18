
#include "panel.h"
#include "../render/font.h"

Panel::Panel(float x, float y, float w, float h, const std::string& title) {

	m_pos   = glm::vec2(x, y);
	m_size  = glm::vec2(w, h);
	m_title = title;
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

bool Panel::headerContains(glm::vec2 p, const Theme& theme) const {

	return p.x >= m_pos.x && p.x < m_pos.x + m_size.x
		&& p.y >= m_pos.y && p.y < m_pos.y + theme.headerHeight;
}

int Panel::update(const UIInput& input, bool mouseOver, const Theme& theme) {

	glm::vec2 local = input.mouse - m_pos;

	m_hoveredElement = -1;
	if (mouseOver && !headerContains(input.mouse, theme)) {

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

void Panel::draw(Batch2d& batch, const Font& font, const Theme& theme) const {

	batch.rect(m_pos.x, m_pos.y, m_size.x, m_size.y, theme.panelBg);
	batch.rect(m_pos.x, m_pos.y, m_size.x, theme.headerHeight, theme.panelHeader);

	float titleBaseline = m_pos.y + (theme.headerHeight + font.size() * 0.7f) * 0.5f;
	font.drawText(batch, m_pos.x + 10.0f, titleBaseline, m_title, theme.text);

	for (int i = 0; i < (int)m_elements.size(); ++i) {
		
		const Element& e = m_elements[i];

		glm::vec4 bg = theme.button;
		glm::vec4 icon = theme.icon;

		if (i == m_pressedElement && i == m_hoveredElement) { bg = theme.buttonPressed; icon = theme.iconPressed; }
		else if (i == m_hoveredElement) { bg = theme.buttonHover;   icon = theme.iconHover; }

		float ex = m_pos.x + e.pos.x;
		float ey = m_pos.y + e.pos.y;
		batch.rect(ex, ey, e.size.x, e.size.y, bg);   

		bool hasIcon = e.icon.texture != 0;
		bool hasLabel = !e.label.empty();

		glm::vec2 iconSize = (e.icon.size.x > 0.0f) ? e.icon.size : glm::vec2(theme.iconSize);

		float iconRight = ex;
		if (hasIcon) {

			float ix = hasLabel ? ex + theme.padding : ex + (e.size.x - iconSize.x) * 0.5f;
			float iy = ey + (e.size.y - iconSize.y) * 0.5f;

			batch.texturedRect(ix, iy, iconSize.x, iconSize.y, e.icon.texture, e.icon.uvMin, e.icon.uvMax, icon);
			iconRight = ix + iconSize.x;
		}

		// label: centered if alone, right after the icon if there is one
		if (hasLabel) {
		
			float tx = hasIcon ? iconRight + theme.iconLabelGap : ex + (e.size.x - font.textWidth(e.label)) * 0.5f;
			
			float ty = ey + (e.size.y + font.size() * 0.7f) * 0.5f;
			font.drawText(batch, tx, ty, e.label, theme.text);
		}
	}
}