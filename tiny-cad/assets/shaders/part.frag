#version 460 core

const vec3 LightColor = vec3(1.0, 0.98, 0.95);

uniform vec3 uCameraPos;
uniform vec3 uColor;

in vec3 vWorldPos;
in vec3 vNormal;

out vec4 FragColor;

void main() {

	vec3 norm = normalize(vNormal);
	vec3 lightDirection = normalize(uCameraPos - vWorldPos);
	
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * LightColor;
	vec3 ambient = 0.1 * LightColor;

	vec3 result = (ambient + diffuse) * uColor;
	FragColor = vec4(result, 1.0);
}