#version 450

uniform mat4 mvp;
uniform mat4 modelView;
uniform mat3 normalMat;
uniform bool invertNormal;

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec3 normal;

layout (location = 0) out vec3 V;
layout (location = 1) out vec3 N;

void main(void)
{
	vec3 finalNormal = normal;
	
	if(invertNormal)
		finalNormal = -normal;

	V = vec3( modelView * vertex );
	N = normalize( normalMat * finalNormal );
	gl_Position = mvp * vertex;
}
