#version 450

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec3 normal;

uniform mat4 mvp;
uniform mat4 modelViewMat;
uniform mat3 normalMat;

layout (location = 0) out vec4 viewPosition;
layout (location = 1) out vec3 viewNormal;

void main()
{
	gl_Position = mvp * vertex;
	viewNormal = normalMat * normal;
	viewPosition = modelViewMat * vertex;
}