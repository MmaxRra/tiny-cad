#pragma once

#include <glad/glad.h>
#include "../cad/meshData.h"

class Mesh {

public:

	explicit Mesh(const MeshData& mesh);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	void draw() const;

private:

	GLsizei m_indexCount = 0;
	GLuint  m_vao = 0;
	GLuint  m_vbo = 0;
	GLuint  m_ebo = 0;
};