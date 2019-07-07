#version 450

layout (location = 0) in vec4 color;
layout (location = 0) out vec4 outColor;


void main(void)
{
	outColor = vec4(color.xyz, 1.0);
}
