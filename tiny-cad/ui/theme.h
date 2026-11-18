#pragma once

#include <glm/glm.hpp>

struct Theme {

	// --- panels ---------------------------------------------------------
	glm::vec4 panelBg;				// panel body
	glm::vec4 panelHeader;			// header strip
	glm::vec4 panelBorder;			// thin outline / separators (for later)

	// --- text -----------------------------------------------------------
	glm::vec4 text;					// normal text, titles, labels
	glm::vec4 textDim;				// hints, secondary info
	glm::vec4 textDisabled;			// disabled elements (for later)

	// --- buttons --------------------------------------------------------
	glm::vec4 button;
	glm::vec4 buttonHover;
	glm::vec4 buttonPressed;

	// --- icons (multiplied with white icon images) ----------------------
	glm::vec4 icon;
	glm::vec4 iconHover;
	glm::vec4 iconPressed;

	// --- accents --------------------------------------------------------
	glm::vec4 accent;				// selected item, active tool, focused field
	glm::vec4 danger;				// close button hover, errors, failed features

	// --- sizes (pixels, at 100% scale) ----------------------------------
	float headerHeight = 22.0f;		// panel header strip
	float padding = 8.0f;			// space inside panels / buttons
	float spacing = 6.0f;			// space between elements
	float buttonHeight = 26.0f;		// standard button height
	float iconSize = 16.0f;			// icons inside buttons
	float iconLabelGap = 6.0f;		// space between an icon and its label
	float fontSize = 15.0f;			// matches the baked font size
};

inline Theme ThemeDark() {

	Theme t;

	t.panelBg       = { 0.18f, 0.19f, 0.22f, 0.96f };
	t.panelHeader   = { 0.22f, 0.23f, 0.27f, 1.00f };
	t.panelBorder   = { 0.28f, 0.29f, 0.34f, 1.00f };

	t.text          = { 0.88f, 0.89f, 0.92f, 1.00f };
	t.textDim       = { 0.60f, 0.62f, 0.67f, 1.00f };
	t.textDisabled  = { 0.42f, 0.43f, 0.47f, 1.00f };

	t.button        = { 0.25f, 0.26f, 0.30f, 1.00f };
	t.buttonHover   = { 0.31f, 0.33f, 0.38f, 1.00f };
	t.buttonPressed = { 0.21f, 0.22f, 0.26f, 1.00f };

	t.icon		    = { 0.80f, 0.81f, 0.85f, 1.00f };
	t.iconHover	    = { 1.00f, 1.00f, 1.00f, 1.00f };
	t.iconPressed   = { 0.65f, 0.66f, 0.70f, 1.00f };

	t.accent = { 0.36f, 0.56f, 0.96f, 1.00f };   // muted blue, sits well next to the axis colors
	t.danger = { 0.86f, 0.32f, 0.32f, 1.00f };   // muted red, same family as the X axis

	return t;
}

inline Theme ThemeLight() {

	// TODO: light values
	Theme t = ThemeDark();   
	return t;
}