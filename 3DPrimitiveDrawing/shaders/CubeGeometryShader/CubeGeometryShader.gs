#version 450

layout ( points ) in;
layout ( triangle_strip, max_vertices = 24) out;

layout (location = 0) in vec3 color[];
layout (location = 0) out vec3 outColor;

uniform mat4 mvp;
uniform mat4 modelViewMat;
uniform mat3 normalMat;
uniform float hLen;


vec3 CalcFragColor(vec4 vertex, vec3 normal)
{
	vec3 V = vec3( modelViewMat * vertex );
	vec3 N = normalize( normalMat * normal );

	vec3 L = normalize(-V);
	return color[0] * max(dot(N,L), 0.0) + vec3(0.1,0.1,0.1);

	//return color[0];
}


void main(void)
{	
	vec4 pos = gl_in[0].gl_Position;

	//front face
	vec4 pos1 = vec4(pos.x - hLen, pos.y + hLen, pos.z + hLen, 1.0);
	vec4 pos2 = vec4(pos.x + hLen, pos.y + hLen, pos.z + hLen, 1.0);
	vec4 pos3 = vec4(pos.x - hLen, pos.y - hLen, pos.z + hLen, 1.0);
	vec4 pos4 = vec4(pos.x + hLen, pos.y - hLen, pos.z + hLen, 1.0);

	outColor = CalcFragColor(pos1, vec3(0.0, 0.0, 1.0));

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();

	
	//back face
	pos1 = vec4(pos.x - hLen, pos.y + hLen, pos.z - hLen, 1.0);
	pos2 = vec4(pos.x + hLen, pos.y + hLen, pos.z - hLen, 1.0);
	pos3 = vec4(pos.x - hLen, pos.y - hLen, pos.z - hLen, 1.0);
	pos4 = vec4(pos.x + hLen, pos.y - hLen, pos.z - hLen, 1.0);

	outColor = CalcFragColor(pos1, vec3(0.0, 0.0, -1.0));

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();

	
	//left face
	pos1 = vec4(pos.x - hLen, pos.y + hLen, pos.z + hLen, 1.0);
	pos2 = vec4(pos.x - hLen, pos.y + hLen, pos.z - hLen, 1.0);
	pos3 = vec4(pos.x - hLen, pos.y - hLen, pos.z + hLen, 1.0);
	pos4 = vec4(pos.x - hLen, pos.y - hLen, pos.z - hLen, 1.0);

	outColor = CalcFragColor(pos1, vec3(-1.0, 0.0, 0.0));

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();
	

	//right face
	pos1 = vec4(pos.x + hLen, pos.y + hLen, pos.z + hLen, 1.0);
	pos2 = vec4(pos.x + hLen, pos.y + hLen, pos.z - hLen, 1.0);
	pos3 = vec4(pos.x + hLen, pos.y - hLen, pos.z + hLen, 1.0);
	pos4 = vec4(pos.x + hLen, pos.y - hLen, pos.z - hLen, 1.0);

	outColor = CalcFragColor(pos1, vec3(1.0, 0.0, 0.0));

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();

	
	//bottom face
	pos1 = vec4(pos.x - hLen, pos.y - hLen, pos.z + hLen, 1.0);
	pos2 = vec4(pos.x + hLen, pos.y - hLen, pos.z + hLen, 1.0);
	pos3 = vec4(pos.x - hLen, pos.y - hLen, pos.z - hLen, 1.0);
	pos4 = vec4(pos.x + hLen, pos.y - hLen, pos.z - hLen, 1.0);

	outColor = CalcFragColor(pos1, vec3(0.0, -1.0, 0.0));

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();


	//top face
	pos1 = vec4(pos.x - hLen, pos.y + hLen, pos.z + hLen, 1.0);
	pos2 = vec4(pos.x + hLen, pos.y + hLen, pos.z + hLen, 1.0);
	pos3 = vec4(pos.x - hLen, pos.y + hLen, pos.z - hLen, 1.0);
	pos4 = vec4(pos.x + hLen, pos.y + hLen, pos.z - hLen, 1.0);

	outColor = CalcFragColor(pos1, vec3(0.0, 1.0, 0.0));

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();
}
