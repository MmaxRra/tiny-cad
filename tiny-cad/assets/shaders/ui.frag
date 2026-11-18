#version 460 core

in vec4 vColor;
in vec2 vUV;

layout(binding = 0) uniform sampler2D uTexture;
out vec4 FragColor;

void main() {

	FragColor = vColor * texture(uTexture, vUV);
}