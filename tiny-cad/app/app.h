#pragma once

#include <glad/glad.h>

#include "../render/partRenderer.h"
#include "../render/axisLines.h"
#include "../core/frameTimer.h"
#include "../cad/primitives.h"
#include "../render/camera.h"
#include "../render/shader.h"
#include "../render/grid.h"
#include "../render/mesh.h"
#include "../core/input.h"

#include "../ui/panelManager.h"
#include "../render/batch2d.h"
#include "../render/texture.h"
#include "../render/font.h"
#include "../ui/theme.h"

class Window;
class App {

public:

	explicit App(Window& window);
	~App() = default;

	App(const App&) = delete;
	App& operator=(const App&) = delete;

	void run();

private:

	void update(float dt);
	void render(int width, int height);
	void everySecond();

	static constexpr glm::vec3 BoxSize{ 40.0f, 20.0f, 30.0f };

	Font    m_font{ "Inter-Regular.ttf", 18.0f };
	Texture m_iconReset{ "icons/test.png" };

	Window&      m_window;
	Camera	     m_camera;

	FrameTimer	 m_timer;
	Input		 m_input;

	AxisLines    m_axes;
	GridRenderer m_grid;

	Batch2d      m_overlay;
	Theme		 m_theme = ThemeDark();
	PanelManager m_panels{ m_theme };
	
	PartRenderer m_part;
	Mesh         m_partMesh;
	glm::mat4    m_partModel;

	void testCallbackA();
	void testCallbackB();
};