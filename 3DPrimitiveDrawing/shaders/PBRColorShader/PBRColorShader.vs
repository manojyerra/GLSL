#version 450

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

uniform mat4 mvp;
uniform mat4 modelViewMat;
uniform mat3 normalMat;

layout (location = 0) out vec4 viewPosition;
layout (location = 1) out vec3 viewNormal;
layout (location = 2) out vec3 viewColor;

out vec4 vertexVary;

void main()
{
	gl_Position = mvp * vertex;
	viewNormal = normalMat * normal;
	viewPosition = modelViewMat * vertex;
	viewColor = vec3(color.r/255.0, color.g/255.0, color.b/255.0);
	
	vertexVary = vertex;
}