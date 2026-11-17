
#include "panel.h"

Panel::Panel(float x, float y, float w, float h, glm::vec4 aColor) {

	m_pos   = glm::vec2(x, y);
	m_size  = glm::vec2(w, h);
	m_Color = aColor;
}

void Panel::draw(Batch2d& batch) const {

	batch.rect(m_pos.x, m_pos.y, m_size.x, m_size.y, m_Color);
	
	glm::vec4 headerColor = m_Color + glm::vec4(0.05f, 0.05f, 0.06f, 0.0f);
	batch.rect(m_pos.x, m_pos.y, m_size.x, m_headerHeight, headerColor);
}