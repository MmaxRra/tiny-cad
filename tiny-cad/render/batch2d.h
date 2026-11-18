#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include <cstdint>
#include "shader.h"

struct Vertex2D {
	glm::vec2 uiPos;
	glm::vec2 uvPos;
	glm::vec4 aColor;
};

class Batch2d {

public:

	Batch2d();
	~Batch2d();

	Batch2d(const Batch2d&) = delete;
	Batch2d& operator=(const Batch2d&) = delete;

	void useTexture(GLuint texture);
	
	void rect(float x, float y, float w, float h, const glm::vec4& color);
	void texturedRect(float x, float y, float w, float h, GLuint texture, glm::vec2 uvMin, glm::vec2 uvMax, const glm::vec4& color);

	void begin(float width, float height);
	void drawFrame();

private:

	static constexpr uint32_t maxQuads = 5000;

	std::vector<Vertex2D> vertices;
	std::vector<uint32_t> indices;

	ShaderProgram m_shader{ "assets/shaders/ui" };
	glm::mat4 m_projection{ 1.0f };

	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;

	GLuint m_whiteTexture = 0;
	GLuint m_currentTexture = 0;

	void flush();
	void populateIndices();
};