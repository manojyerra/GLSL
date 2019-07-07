#version 450

layout (location = 0) out vec4 outColor;

layout (location = 0) in vec3 color;

void main(void)
{
	outColor = vec4(color, 1.0);
}
