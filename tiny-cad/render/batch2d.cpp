
#include "batch2d.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cstddef>

Batch2d::Batch2d() { 
	
	populateIndices();

	glCreateBuffers(1, &m_vbo);
	glNamedBufferStorage(m_vbo, maxQuads * 4 * sizeof(Vertex2D), nullptr, GL_DYNAMIC_STORAGE_BIT);

	glCreateVertexArrays(1, &m_vao);
	glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(Vertex2D));

	glCreateBuffers(1, &m_ebo);
	glNamedBufferStorage(m_ebo, maxQuads * 6 * sizeof(uint32_t), indices.data(), 0);

	glVertexArrayElementBuffer(m_vao, m_ebo);
	
	// location 0: position
	glEnableVertexArrayAttrib(m_vao, 0);
	glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, uiPos));
	glVertexArrayAttribBinding(m_vao, 0, 0);

	// location 1: acolor
	glEnableVertexArrayAttrib(m_vao, 1);
	glVertexArrayAttribFormat(m_vao, 1, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, aColor));
	glVertexArrayAttribBinding(m_vao, 1, 0);
}

Batch2d::~Batch2d() { 

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void Batch2d::begin(float width, float height) {

	vertices.clear();
	m_projection = glm::ortho(0.0f, width, height, 0.0f);
}

void Batch2d::rect(float x, float y, float w, float h, const glm::vec4& color) {

	if (vertices.size() / 4 >= maxQuads) flush();

	vertices.push_back({ { x,     y     }, color });
	vertices.push_back({ { x + w, y     }, color });
	vertices.push_back({ { x + w, y + h }, color });
	vertices.push_back({ { x,     y + h }, color });
}

void Batch2d::drawFrame() { flush(); }

void Batch2d::flush() { 

	glNamedBufferSubData(m_vbo, 0, vertices.size() * sizeof(Vertex2D), vertices.data());
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_shader.setMat4("uProjection", m_projection);
	m_shader.use();

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, (GLsizei)(vertices.size() / 4 * 6), GL_UNSIGNED_INT, nullptr);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	vertices.clear();
}

void Batch2d::populateIndices() {

	indices.reserve(maxQuads * 6);
	for (uint32_t q = 0; q < maxQuads; ++q) {
		
		uint32_t b = q * 4;
		indices.insert(indices.end(), { b + 0, b + 1, b + 2, b + 2, b + 3, b + 0 });
	}
}