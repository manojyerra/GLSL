#version 450

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec3 normal;

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;
uniform mat3 normalMat;

layout (location = 0) out vec4 viewPosition;
layout (location = 1) out vec3 viewNormal;

void main()
{
	gl_Position = projMat * viewMat * modelMat * vertex;
	viewNormal = normalMat * normal;
	viewPosition = viewMat * modelMat * vertex;
}