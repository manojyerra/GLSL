#version 450

uniform mat4 mvp;

layout (location = 0) in vec4 vertex;

void main (void)  
{     
   gl_Position = mvp * vertex;
}
