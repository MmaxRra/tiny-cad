
#include "axisLines.h"

AxisLines::AxisLines(float length)
{
	const float L = length;

	// position (x, y, z)     color (r, g, b)
	const float vertices[] = {
		0, 0, 0,   1, 0, 0,     L, 0, 0,   1, 0, 0,   // X - red
		0, 0, 0,   0, 1, 0,     0, L, 0,   0, 1, 0,   // Y - green (up)
		0, 0, 0,   0, 0, 1,     0, 0, L,   0, 0, 1,   // Z - blue
	};

	glCreateBuffers(1, &m_vbo);
	glNamedBufferStorage(m_vbo, sizeof(vertices), vertices, 0);

	glCreateVertexArrays(1, &m_vao);
	glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, 6 * sizeof(float));

	// location 0: position
	glEnableVertexArrayAttrib(m_vao, 0);
	glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(m_vao, 0, 0);

	// location 1: color
	glEnableVertexArrayAttrib(m_vao, 1);
	glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
	glVertexArrayAttribBinding(m_vao, 1, 0);
}

AxisLines::~AxisLines()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void AxisLines::draw(const glm::mat4& viewProj) const
{
	m_shader.setMat4("uViewProj", viewProj);
	m_shader.use();

	glBindVertexArray(m_vao);
	glDrawArrays(GL_LINES, 0, 6);
}