#version 450

in vec3 vertex;
in vec3 color;

layout (location = 0) out vec3 outColor;

void main(void)
{
	gl_Position = vec4(vertex,1.0);
	outColor = vec3(color.x/255.0, color.y/255.0, color.z/255.0);
}
