#version 450

uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modelMat;

layout (location = 0) in vec4 vertex;

void main (void)  
{     
   gl_Position = projMat * viewMat * modelMat * vertex;
}
