
#include "primitives.h"

static void appendQuadIndices(MeshData& md, uint32_t firstVertex, uint32_t quadCount) {

	for (uint32_t quad = 0; quad < quadCount; ++quad) {
		uint32_t base = firstVertex + quad * 4;
		md.indices.insert(md.indices.end(), {
			base + 0, base + 1, base + 2,
			base + 2, base + 3, base + 0
			});
	}
}

MeshData makeBox(glm::vec3 size) {

	const glm::vec3 h = size * 0.5f;
	MeshData md;

	md.vertices = {
		// +Z (front)
		{ { -h.x, -h.y,  h.z }, {  0,  0,  1 } },
		{ {  h.x, -h.y,  h.z }, {  0,  0,  1 } },
		{ {  h.x,  h.y,  h.z }, {  0,  0,  1 } },
		{ { -h.x,  h.y,  h.z }, {  0,  0,  1 } },

		// -Z (back)
		{ {  h.x, -h.y, -h.z }, {  0,  0, -1 } },
		{ { -h.x, -h.y, -h.z }, {  0,  0, -1 } },
		{ { -h.x,  h.y, -h.z }, {  0,  0, -1 } },
		{ {  h.x,  h.y, -h.z }, {  0,  0, -1 } },

		// +X (right)
		{ {  h.x, -h.y,  h.z }, {  1,  0,  0 } },
		{ {  h.x, -h.y, -h.z }, {  1,  0,  0 } },
		{ {  h.x,  h.y, -h.z }, {  1,  0,  0 } },
		{ {  h.x,  h.y,  h.z }, {  1,  0,  0 } },

		// -X (left)
		{ { -h.x, -h.y, -h.z }, { -1,  0,  0 } },
		{ { -h.x, -h.y,  h.z }, { -1,  0,  0 } },
		{ { -h.x,  h.y,  h.z }, { -1,  0,  0 } },
		{ { -h.x,  h.y, -h.z }, { -1,  0,  0 } },

		// +Y (top)
		{ { -h.x,  h.y,  h.z }, {  0,  1,  0 } },
		{ {  h.x,  h.y,  h.z }, {  0,  1,  0 } },
		{ {  h.x,  h.y, -h.z }, {  0,  1,  0 } },
		{ { -h.x,  h.y, -h.z }, {  0,  1,  0 } },

		// -Y (bottom)
		{ { -h.x, -h.y, -h.z }, {  0, -1,  0 } },
		{ {  h.x, -h.y, -h.z }, {  0, -1,  0 } },
		{ {  h.x, -h.y,  h.z }, {  0, -1,  0 } },
		{ { -h.x, -h.y,  h.z }, {  0, -1,  0 } },
	};

	// append indices
	appendQuadIndices(md, 0, 6);
	return md;
}