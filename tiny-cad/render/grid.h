#pragma once

#include <glad/glad.h>
#include "shader.h"
#include "camera.h"

class GridRenderer {

public:

	// todo: for the future we pass a type of grid so 
	//		 we could render different types of grids
	explicit GridRenderer();
	~GridRenderer();

	GridRenderer(const GridRenderer&) = delete;
	GridRenderer& operator=(const GridRenderer&) = delete;

	void draw(Camera& camera) const;

private:

	ShaderProgram m_shader{ "assets/shaders/grid" };
	GLuint		  m_vao = 0;
};