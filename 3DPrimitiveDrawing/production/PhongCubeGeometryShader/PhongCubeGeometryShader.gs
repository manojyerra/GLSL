#version 450

layout ( points ) in;
layout ( triangle_strip, max_vertices = 24) out;

layout (location = 0) in vec3 inColor[];
layout (location = 0) out vec3 outColor;

uniform mat4 mvp;
uniform float hLen;

void main(void)
{		
	vec4 pos = gl_in[0].gl_Position;
	
	float xPVe = pos.x + hLen;
	float xNVe = pos.x - hLen;
	float yPVe = pos.y + hLen;
	float yNVe = pos.y - hLen;
	float zPVe = pos.z + hLen;
	float zNVe = pos.z - hLen;
	
	outColor = inColor[0];
		
	//front face
	vec4 pos1 = vec4(xNVe, yPVe, zPVe, 1.0);
	vec4 pos2 = vec4(xPVe, yPVe, zPVe, 1.0);
	vec4 pos3 = vec4(xNVe, yNVe, zPVe, 1.0);
	vec4 pos4 = vec4(xPVe, yNVe, zPVe, 1.0);

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();

	
	//back face
	pos1 = vec4(xNVe, yPVe, zNVe, 1.0);
	pos2 = vec4(xPVe, yPVe, zNVe, 1.0);
	pos3 = vec4(xNVe, yNVe, zNVe, 1.0);
	pos4 = vec4(xPVe, yNVe, zNVe, 1.0);

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();

	
	//left face
	pos1 = vec4(xNVe, yPVe, zPVe, 1.0);
	pos2 = vec4(xNVe, yPVe, zNVe, 1.0);
	pos3 = vec4(xNVe, yNVe, zPVe, 1.0);
	pos4 = vec4(xNVe, yNVe, zNVe, 1.0);

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();
	

	//right face
	pos1 = vec4(xPVe, yPVe, zPVe, 1.0);
	pos2 = vec4(xPVe, yPVe, zNVe, 1.0);
	pos3 = vec4(xPVe, yNVe, zPVe, 1.0);
	pos4 = vec4(xPVe, yNVe, zNVe, 1.0);

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();

	
	//bottom face
	pos1 = vec4(xNVe, yNVe, zPVe, 1.0);
	pos2 = vec4(xPVe, yNVe, zPVe, 1.0);
	pos3 = vec4(xNVe, yNVe, zNVe, 1.0);
	pos4 = vec4(xPVe, yNVe, zNVe, 1.0);

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();


	//top face
	pos1 = vec4(xNVe, yPVe, zPVe, 1.0);
	pos2 = vec4(xPVe, yPVe, zPVe, 1.0);
	pos3 = vec4(xNVe, yPVe, zNVe, 1.0);
	pos4 = vec4(xPVe, yPVe, zNVe, 1.0);

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();
}
