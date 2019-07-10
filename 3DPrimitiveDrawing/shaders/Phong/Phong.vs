#version 450

uniform mat4 mvp;
uniform mat4 modelView;
uniform mat3 normalMat;

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec3 normal;

layout (location = 0) out vec3 V;
layout (location = 1) out vec3 N;

void main(void)
{
	V = vec3( modelView * vertex );
	N = normalize( normalMat * normal );
	gl_Position = mvp * vertex;
}
