
#include "grid.h"

GridRenderer::GridRenderer() { glCreateVertexArrays(1, &m_vao); }
GridRenderer::~GridRenderer() { glDeleteVertexArrays(1, &m_vao); }

void GridRenderer::draw(Camera& camera) const {

	m_shader.setMat4("uViewProj", camera.getViewProjection());
	m_shader.setFloat("uCamToTargetDist", camera.getDistance());
	m_shader.setFloat("uGridSize", 10000.0f);
	m_shader.use();

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}