#version 450

uniform mat4 mvp;

//in variables...
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 color;

//out variables...
layout (location = 0) out vec3 color_vary;

void main(void)
{
	color_vary = vec3(color.r/255.0, color.g/255.0, color.b/255.0);
	gl_Position = mvp * vec4(vertex, 1.0);
}
