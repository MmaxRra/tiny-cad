#pragma once

#include "shader.h"
#include "camera.h"
#include "mesh.h"

class PartRenderer {

public:

	PartRenderer() = default;
	~PartRenderer() = default;

	void draw(const Camera& camera, const Mesh& mesh, const glm::mat4& model, const glm::vec3& color) const;

private:

	ShaderProgram m_shader{ "assets/shaders/part" };
};