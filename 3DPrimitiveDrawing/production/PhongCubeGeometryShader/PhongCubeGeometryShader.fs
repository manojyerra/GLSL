#version 450

layout (location = 0) in vec3 inColor;
layout (location = 0) out vec4 outColor;

uniform float alpha;

void main(void)
{
	outColor = vec4(inColor, alpha);
}