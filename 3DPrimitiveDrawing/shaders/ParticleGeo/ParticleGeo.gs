#version 450

layout ( points ) in;
layout ( triangle_strip, max_vertices = 8 ) out;

layout (location = 0) in vec3 color[];
layout (location = 0) out vec3 outColor;

uniform mat4 projMat;
uniform mat4 modelMat;
uniform mat4 viewMat;

void main(void)
{
	vec4 offset = vec4(-1.0, 1.0, 0.0, 0.0);
	vec4 vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projMat * viewMat * modelMat * vertexPos;
	outColor = color[0] * vec3(1.0, 0.0, 0.0);
	EmitVertex();
	
	offset = vec4(-1.0, -1.0, 0.0, 0.0);
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projMat * viewMat * modelMat * vertexPos;
	outColor = color[0] * vec3(0.0, 1.0, 0.0);
	EmitVertex();
	
	offset = vec4(1.0, 1.0, 0.0, 0.0);
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projMat * viewMat * modelMat * vertexPos;
	outColor = color[0] * vec3(1.0, 0.0, 1.0);
	EmitVertex();
	
	offset = vec4(1.0, -1.0, 0.0, 0.0);
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projMat * viewMat * modelMat * vertexPos;
	outColor = color[0] * vec3(0.0, 0.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
	
	offset = vec4(-1.0, 1.0, 1.0, 0.0);
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projMat * viewMat * modelMat * vertexPos;
	outColor = color[0] * vec3(1.0, 0.0, 0.0);
	EmitVertex();
	
	offset = vec4(-1.0, -1.0, 1.0, 0.0);
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projMat * viewMat * modelMat * vertexPos;
	outColor = color[0] * vec3(0.0, 1.0, 0.0);
	EmitVertex();
	
	offset = vec4(1.0, 1.0, 1.0, 0.0);
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projMat * viewMat * modelMat * vertexPos;
	outColor = color[0] * vec3(1.0, 0.0, 1.0);
	EmitVertex();
	
	offset = vec4(1.0, -1.0, 1.0, 0.0);
	vertexPos = offset + gl_in[0].gl_Position;
	gl_Position = projMat * viewMat * modelMat * vertexPos;
	outColor = color[0] * vec3(0.0, 0.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
}