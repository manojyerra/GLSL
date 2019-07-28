#version 450

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;

in vec3 vertexVary;
in vec3 normalVary;

void main()
{
    gPosition = vertexVary;
    gNormal = normalVary;
}