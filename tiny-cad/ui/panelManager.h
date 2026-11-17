#pragma once

#include <vector>

#include "../render/batch2d.h"
#include "panel.h"

class PanelManager {

public:

	PanelManager() = default;
	~PanelManager() = default;

	void addPanel(float x, float y, float w, float h, const glm::vec4& color);
	void draw(Batch2d& batch);

private:

	std::vector<Panel> m_panels;
};