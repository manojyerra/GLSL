#version 450

in vec3 vertex;
in vec3 color;

layout (location = 0) out vec3 outColour;

void main(void)
{
	gl_Position = vec4(vertex,1.0);
	outColour = color;
}
