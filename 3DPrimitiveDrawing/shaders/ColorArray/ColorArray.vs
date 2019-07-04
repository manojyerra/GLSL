#version 450

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

//in variables...
layout (location = 0) in vec4 vertex;
layout (location = 1) in vec4 color;

//out variables...
layout (location = 0) out vec4 fragColor;

void main(void)
{
	fragColor = color;
	gl_Position = projMat * viewMat * modelMat * vertex;
}


/*
uniform Transform
{
 mat4 projMat;
 mat4 modelMat;
 mat4 oriMat;
} trans;
*/