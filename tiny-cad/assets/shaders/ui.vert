#version 460 core

layout(location = 0) in vec2 uiPos;
layout(location = 1) in vec2 uvPos;
layout(location = 2) in vec4 aColor;
uniform mat4 uProjection;

out vec2 vUV;
out vec4 vColor;

void main() {

	vColor = aColor;
	vUV = uvPos;

	gl_Position = uProjection * vec4(uiPos, 0.0, 1.0f);
}
