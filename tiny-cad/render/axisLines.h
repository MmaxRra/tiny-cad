#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"

class AxisLines {

public:

	explicit AxisLines(float length = 1.0f);
	~AxisLines();

	AxisLines(const AxisLines&) = delete;
	AxisLines& operator=(const AxisLines&) = delete;

	void draw(const glm::mat4& viewProj) const;

private:

	ShaderProgram m_shader{ "assets/shaders/line" };
	GLuint        m_vao = 0;
	GLuint        m_vbo = 0;
};