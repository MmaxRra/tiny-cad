#version 460 core

in vec2 vUV;
out vec4 FragColor;

uniform float uTime;

void main()
{
    // Animated gradient. Try editing this line while the app runs:
    vec3 color = 0.5 + 0.5 * cos(uTime + vUV.xyx * 3.0 + vec3(0.0, 2.0, 4.0));

    FragColor = vec4(color, 1.0);
}
