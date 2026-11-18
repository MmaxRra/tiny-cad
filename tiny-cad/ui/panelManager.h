#pragma once

#include <vector>

#include "../render/batch2d.h"
#include "theme.h"
#include "panel.h"

class Font;
class PanelManager {

public:

	explicit  PanelManager(const Theme& theme) : m_theme(theme) {}
	~PanelManager() = default;

	Panel& addPanel(float x, float y, float w, float h, const std::string& title);
	
	bool update(const UIInput& input);
	void draw(Batch2d& batch, const Font& font) const;

private:

	std::vector<Panel> m_panels;
	const Theme& m_theme;

	int  m_interactiveIndex = -1;
	bool m_isDragging = false;
	// bool m_isFocused  = false;

	glm::vec2 m_dragOffset{ 0.0f };
};