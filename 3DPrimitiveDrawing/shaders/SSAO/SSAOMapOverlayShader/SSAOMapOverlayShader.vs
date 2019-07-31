#version 450

layout(location = 0) in vec2 uv;
layout(location = 1) in vec3 vertex;

layout(location = 0) out vec2 uvVary;


void main(void)
{
	uvVary = uv;
	gl_Position = vec4(vertex, 1.0);
}
