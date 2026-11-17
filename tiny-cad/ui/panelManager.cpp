
#include "panelManager.h"

void PanelManager::addPanel(float x, float y, float w, float h, const glm::vec4& color) {

	m_panels.emplace_back(x, y, w, h, color);
}

void PanelManager::draw(Batch2d& batch) {

	for (Panel& p : m_panels) { p.draw(batch); }
}