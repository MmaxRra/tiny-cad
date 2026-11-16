#pragma once

#include <glm/glm.hpp>
#include <cstdint>
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
};

struct MeshData {

	std::vector<Vertex>   vertices;
	std::vector<uint32_t> indices;
};