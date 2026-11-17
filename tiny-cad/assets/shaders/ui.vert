#version 460 core

layout(location = 0) in vec3 uiPos;
layout(location = 1) in vec4 aColor;
uniform mat4 uProjection;

out vec4 vColor;

void main() {

	vColor = aColor;
	gl_Position = uProjection * vec4(uiPos, 1.0f);
}
