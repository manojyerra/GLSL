#version 450

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec2 uv;

out vec2 TexCoords;

void main()
{
    TexCoords = uv;
    gl_Position = vertex;
}