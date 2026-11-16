#pragma once

#include "meshData.h"

// helpers
static void appendQuadIndices(MeshData& md, uint32_t firstVertex, uint32_t quadCount);

MeshData makeBox(glm::vec3 size);