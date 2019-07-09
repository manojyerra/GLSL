#version 450

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;
uniform mat4 scaleMat;

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 color;

layout (location = 0) out vec3 fragColor;

void main(void)
{
	fragColor = color;
	gl_Position = projMat * viewMat * modelMat * scaleMat * vec4(vertex, 1.0);
}
