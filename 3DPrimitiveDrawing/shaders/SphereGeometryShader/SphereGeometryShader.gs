#version 450

layout ( points ) in;
layout ( triangle_strip, max_vertices = 4) out;

layout (location = 0) in vec3 inColor[];

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec3 outPos;
layout (location = 2) out vec3 outCenPos;

uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform mat3 normalMat;
uniform float hLen;


void main(void)
{	
	outColor = inColor[0];

	vec4 pos = modelViewMat * gl_in[0].gl_Position;
	
	//front face
	vec4 pos1 = vec4(pos.x - hLen, pos.y + hLen, pos.z, 1.0);
	vec4 pos2 = vec4(pos.x + hLen, pos.y + hLen, pos.z, 1.0);
	vec4 pos3 = vec4(pos.x - hLen, pos.y - hLen, pos.z, 1.0);
	vec4 pos4 = vec4(pos.x + hLen, pos.y - hLen, pos.z, 1.0);

	outCenPos = vec3(pos);
	
	gl_Position = projMat * pos1; 
	outPos = vec3(pos1);
	EmitVertex();
	
	gl_Position = projMat * pos2; 
	outPos = vec3(pos2);
	EmitVertex();
	
	gl_Position = projMat * pos3; 
	outPos = vec3(pos3);
	EmitVertex();
	
	gl_Position = projMat * pos4; 
	outPos = vec3(pos4);
	EmitVertex();
	
	EndPrimitive();
}
