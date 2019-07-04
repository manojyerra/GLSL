#version 450

uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat4 oriMat;

layout (location = 0) in vec4 color;
layout (location = 1) in vec3 vertex;

layout (location = 0) out vec4 fragColor;

void main(void)
{
	fragColor = color;
	gl_Position = projMat * modelMat * oriMat * vec4(vertex, 1.0);
}


/*

uniform Transform
{
 mat4 projMat;
 mat4 modelMat;
 mat4 oriMat;
} trans;

*/