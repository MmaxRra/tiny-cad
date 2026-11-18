
#include "panelManager.h"
#include <algorithm>

Panel& PanelManager::addPanel(float x, float y, float w, float h, const glm::vec4& color) {

	return m_panels.emplace_back(x, y, w, h, color);
}

bool PanelManager::update(const UIInput input) {

	int hovered = -1;
	for (int i = (int)m_panels.size() - 1; i >= 0; --i) {
		
		if (m_panels[i].contains(input.mouse)) {
			
			hovered = i;
			break;
		}
	}

	if (input.pressed && hovered != -1) {
		
		std::rotate(m_panels.begin() + hovered, m_panels.begin() + hovered + 1, m_panels.end());
		
		hovered = (int)m_panels.size() - 1;
		m_interactiveIndex = hovered;

		if (m_panels[hovered].headerContains(input.mouse)) {

			m_isDragging = true;
			m_dragOffset = m_panels[hovered].getPosition() - input.mouse;
		}
	}

	if (m_isDragging && input.held) {
		m_panels[m_interactiveIndex].setPosition(input.mouse + m_dragOffset);
	}

	if (input.released) {
		m_isDragging = false;
		m_interactiveIndex = -1;
	}

	for (Panel& p : m_panels) p.clampTo(input.screenSize);

	int clickedPanel = -1, clickedElement = -1;
	for (int i = 0; i < (int)m_panels.size(); ++i) {

		bool mouseOver = (i == hovered) && !m_isDragging;
		int e = m_panels[i].update(input, mouseOver);
		if (e != -1) { clickedPanel = i; clickedElement = e; }
	}

	if (clickedPanel != -1) {

		std::function<void()> onClick = m_panels[clickedPanel].getOnClick(clickedElement);
		if (onClick) onClick();
	}

	return hovered != -1 || m_isDragging;
}

void PanelManager::draw(Batch2d& batch) { for (Panel& p : m_panels) { p.draw(batch); } }