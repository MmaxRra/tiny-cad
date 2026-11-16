
#include "mesh.h"

Mesh::Mesh(const MeshData& mesh) {

	glCreateBuffers(1, &m_vbo);
	glNamedBufferStorage(m_vbo, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), 0);

	glCreateVertexArrays(1, &m_vao);
	glVertexArrayVertexBuffer(m_vao, 0, m_vbo, 0, sizeof(Vertex));

	glCreateBuffers(1, &m_ebo);
	glNamedBufferStorage(m_ebo, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), 0);

	m_indexCount = (GLsizei)mesh.indices.size();
	glVertexArrayElementBuffer(m_vao, m_ebo);

	// location 0: position
	glEnableVertexArrayAttrib(m_vao, 0);
	glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
	glVertexArrayAttribBinding(m_vao, 0, 0);

	// location 1: normal
	glEnableVertexArrayAttrib(m_vao, 1);
	glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
	glVertexArrayAttribBinding(m_vao, 1, 0);
}

Mesh::~Mesh() { 

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void Mesh::draw() const {

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
}