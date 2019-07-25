#version 450

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 vertexVary;
out vec3 normalVary;
out vec2 uvVary;

uniform mat4 mvp;
uniform mat4 modelView;
uniform mat3 normalMat;

void main()
{
    vertexVary = vec3(modelView * vertex);
	normalVary = normalize(normalMat * normal);
    uvVary = uv;
    
    gl_Position = mvp * vertex;
}