#version 450

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

//in variables...
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 color;

//out variables...
layout (location = 0) out vec3 fragColor;

void main(void)
{
	fragColor = color;
	gl_Position = projMat * viewMat * modelMat * vec4(vertex, 1.0);
}


/*
uniform Transform
{
 mat4 projMat;
 mat4 modelMat;
 mat4 oriMat;
} trans;
*/