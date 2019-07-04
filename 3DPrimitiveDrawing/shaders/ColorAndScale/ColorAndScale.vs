#version 450

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;
uniform mat4 scaleMat;

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec4 color;

layout (location = 0) out vec4 fragColor;

void main(void)
{
	fragColor = color;
	gl_Position = projMat * viewMat * modelMat * scaleMat * vertex;
}
