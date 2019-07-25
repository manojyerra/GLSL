#version 450

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 uv;

out vec2 uvVary;

void main()
{
    uvVary = uv;
    gl_Position = vec4(vertex, 1.0);
}