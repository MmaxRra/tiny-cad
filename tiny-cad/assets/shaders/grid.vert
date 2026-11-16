#version 460 core

uniform mat4 uViewProj;
uniform float uGridSize;

out vec3 vWorldPos;

// builds the quad triangle plane
const vec2 points[6] = vec2[](

	vec2( 1.0, -1.0), vec2(-1.0, -1.0), vec2(-1.0,  1.0),
	vec2(-1.0,  1.0), vec2( 1.0,  1.0), vec2( 1.0, -1.0)
);

void main() {
	vec2 p = points[gl_VertexID] * uGridSize;
	vWorldPos = vec3(p.x, 0.0, p.y); 

	gl_Position = uViewProj * vec4(vWorldPos, 1.0);
}