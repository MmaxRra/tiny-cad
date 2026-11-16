
#include "partRenderer.h"

void PartRenderer::draw(const Camera& camera, const Mesh& mesh, const glm::mat4& model, const glm::vec3& color) const {

	m_shader.setMat4("uViewProj", camera.getViewProjection());
	m_shader.setMat4("uModel", model);

	m_shader.setVec3("uCameraPos", camera.getPosition());
	m_shader.setVec3("uColor", color);

	m_shader.use();
	mesh.draw();
}