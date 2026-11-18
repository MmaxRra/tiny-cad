
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
	glVertexArrayAttribFormat(m_vao, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, uiPos));
	glVertexArrayAttribBinding(m_vao, 0, 0);

	// location 1: uv position
	glEnableVertexArrayAttrib(m_vao, 1);
	glVertexArrayAttribFormat(m_vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, uvPos));
	glVertexArrayAttribBinding(m_vao, 1, 0);

	// location 2: acolor
	glEnableVertexArrayAttrib(m_vao, 2);
	glVertexArrayAttribFormat(m_vao, 2, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex2D, aColor));
	glVertexArrayAttribBinding(m_vao, 2, 0);

	// location 0-frag: texture
	const uint32_t white = 0xFFFFFFFF; // rgba pixel

	glCreateTextures(GL_TEXTURE_2D, 1, &m_whiteTexture);
	glTextureStorage2D(m_whiteTexture, 1, GL_RGBA8, 1, 1);
	glTextureSubImage2D(m_whiteTexture, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &white);
}

Batch2d::~Batch2d() { 

	glDeleteTextures(1, &m_whiteTexture);

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void Batch2d::begin(float width, float height) {

	vertices.clear();
	m_projection = glm::ortho(0.0f, width, height, 0.0f);
}

void Batch2d::rect(float x, float y, float w, float h, const glm::vec4& color) {

	useTexture(m_whiteTexture);
	if (vertices.size() / 4 >= maxQuads) flush();

	const glm::vec2 uv{ 0.5f, 0.5f };   // center of the 1x1 white texture
	vertices.push_back({ { x,     y     }, uv, color });
	vertices.push_back({ { x + w, y     }, uv, color });
	vertices.push_back({ { x + w, y + h }, uv, color });
	vertices.push_back({ { x,     y + h }, uv, color });
}

void Batch2d::texturedRect(float x, float y, float w, float h,
	GLuint texture, glm::vec2 uvMin, glm::vec2 uvMax,
	const glm::vec4& color) {

	useTexture(texture);
	if (vertices.size() / 4 >= maxQuads) flush();

	vertices.push_back({ { x,     y     }, { uvMin.x, uvMin.y }, color });
	vertices.push_back({ { x + w, y     }, { uvMax.x, uvMin.y }, color });
	vertices.push_back({ { x + w, y + h }, { uvMax.x, uvMax.y }, color });
	vertices.push_back({ { x,     y + h }, { uvMin.x, uvMax.y }, color });
}

void Batch2d::useTexture(GLuint texture) {
	
	if (texture != m_currentTexture) { flush(); m_currentTexture = texture; }
}

void Batch2d::drawFrame() { flush(); }

void Batch2d::flush() { 

	glNamedBufferSubData(m_vbo, 0, vertices.size() * sizeof(Vertex2D), vertices.data());
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_shader.setMat4("uProjection", m_projection);
	m_shader.use();

	glBindTextureUnit(0, m_currentTexture);
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