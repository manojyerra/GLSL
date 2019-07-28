#version 450

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec2 uv;

out vec2 uvVary;

void main()
{
    uvVary = uv;
    gl_Position = vertex;
}