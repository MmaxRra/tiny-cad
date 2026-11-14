#version 460 core

// Full-screen triangle generated from the vertex index - no vertex buffer needed.
// Vertex 0: (-1,-1), 1: (3,-1), 2: (-1,3) -> covers the whole screen.

out vec2 vUV;

void main()
{
    vec2 pos = vec2((gl_VertexID << 1) & 2, gl_VertexID & 2);
    vUV = pos;
    gl_Position = vec4(pos * 2.0 - 1.0, 0.0, 1.0);
}
