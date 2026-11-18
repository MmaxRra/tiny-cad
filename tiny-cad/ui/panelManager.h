#pragma once

#include <vector>

#include "../render/batch2d.h"
#include "panel.h"

class PanelManager {

public:

	PanelManager() = default;
	~PanelManager() = default;

	Panel& addPanel(float x, float y, float w, float h, const glm::vec4& color);
	
	bool update(const UIInput input);
	void draw(Batch2d& batch);

private:

	std::vector<Panel> m_panels;

	int  m_interactiveIndex = -1;
	bool m_isDragging = false;
	bool m_isFocused  = false;

	glm::vec2 m_dragOffset{ 0.0f };
};