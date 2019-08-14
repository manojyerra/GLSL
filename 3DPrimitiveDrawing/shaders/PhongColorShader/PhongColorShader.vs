#version 450

uniform mat4 mvp;
uniform mat4 modelView;
uniform mat3 normalMat;

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

layout (location = 0) out vec3 V;
layout (location = 1) out vec3 N;
layout (location = 2) out vec3 colorVary;

void main(void)
{
	V = vec3( modelView * vertex );
	N = normalize( normalMat * normal );
	colorVary = vec3(color.r/255.0, color.g/255.0, color.b/255.0);
	gl_Position = mvp * vertex;
}
