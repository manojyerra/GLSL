#version 450

uniform mat4 mvp;

layout(location = 0) in vec2 uv;
layout(location = 1) in vec4 vertex;

layout(location = 0) out vec2 uv_varying;


void main(void)
{
	uv_varying = uv;
	gl_Position = mvp * vertex;
}
