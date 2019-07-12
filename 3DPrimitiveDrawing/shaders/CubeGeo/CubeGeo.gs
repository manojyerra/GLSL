#version 450

layout ( points ) in;
layout ( triangle_strip, max_vertices = 36) out;

layout (location = 0) in vec3 color[];
layout (location = 0) out vec3 outColor;

uniform mat4 mvp;
uniform mat4 modelViewMat;
uniform mat3 normalMat;
uniform float hLen;

uniform int methodNum;

vec3 CalcFragColor(vec4 vertex, vec3 normal)
{
	vec3 V = vec3( modelViewMat * vertex );
	vec3 N = normalize( normalMat * normal );

	vec3 L = normalize(-V);
	return color[0] * max(dot(N,L), 0.0);

	//return color[0];
}

void method1()
{
	vec4 pos = gl_in[0].gl_Position;
	outColor = color[0];

	vec4 pos1 = vec4(pos.x - hLen, pos.y + hLen, pos.z + hLen, 1.0);
	vec4 pos2 = vec4(pos.x + hLen, pos.y + hLen, pos.z + hLen, 1.0);
	vec4 pos3 = vec4(pos.x - hLen, pos.y - hLen, pos.z + hLen, 1.0);
	vec4 pos4 = vec4(pos.x + hLen, pos.y - hLen, pos.z + hLen, 1.0);
	vec4 pos5 = vec4(pos.x - hLen, pos.y - hLen, pos.z - hLen, 1.0);
	vec4 pos6 = vec4(pos.x + hLen, pos.y - hLen, pos.z - hLen, 1.0);
	vec4 pos7 = vec4(pos.x - hLen, pos.y + hLen, pos.z - hLen, 1.0);
	vec4 pos8 = vec4(pos.x + hLen, pos.y + hLen, pos.z - hLen, 1.0);

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	gl_Position = mvp * pos5; EmitVertex();
	gl_Position = mvp * pos6; EmitVertex();
	gl_Position = mvp * pos7; EmitVertex();
	gl_Position = mvp * pos8; EmitVertex();

	EndPrimitive();


	pos1 = vec4(pos.x - hLen, pos.y - hLen, pos.z + hLen, 1.0);
	pos2 = vec4(pos.x - hLen, pos.y - hLen, pos.z - hLen, 1.0);
	pos3 = vec4(pos.x - hLen, pos.y + hLen, pos.z + hLen, 1.0);
	pos4 = vec4(pos.x - hLen, pos.y + hLen, pos.z - hLen, 1.0);
	pos5 = vec4(pos.x + hLen, pos.y + hLen, pos.z + hLen, 1.0);
	pos6 = vec4(pos.x + hLen, pos.y + hLen, pos.z - hLen, 1.0);
	pos7 = vec4(pos.x + hLen, pos.y - hLen, pos.z + hLen, 1.0);
	pos8 = vec4(pos.x + hLen, pos.y - hLen, pos.z - hLen, 1.0);

	gl_Position = mvp * pos1; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	gl_Position = mvp * pos5; EmitVertex();
	gl_Position = mvp * pos6; EmitVertex();
	gl_Position = mvp * pos7; EmitVertex();
	gl_Position = mvp * pos8; EmitVertex();

	EndPrimitive();
}

void method2()
{
	vec4 pos = gl_in[0].gl_Position;

	//front face
	vec4 pos1 = vec4(pos.x - hLen, pos.y + hLen, pos.z + hLen, 1.0);
	vec4 pos2 = vec4(pos.x + hLen, pos.y + hLen, pos.z + hLen, 1.0);
	vec4 pos3 = vec4(pos.x - hLen, pos.y - hLen, pos.z + hLen, 1.0);
	vec4 pos4 = vec4(pos.x + hLen, pos.y - hLen, pos.z + hLen, 1.0);

	outColor = CalcFragColor(pos1, vec3(0.0, 0.0, 1.0));

	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos1; EmitVertex();
	EndPrimitive();

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
	EndPrimitive();

	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
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
	EndPrimitive();

	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();
	

	//right face
	pos1 = vec4(pos.x + hLen, pos.y + hLen, pos.z + hLen, 1.0);
	pos2 = vec4(pos.x + hLen, pos.y + hLen, pos.z - hLen, 1.0);
	pos3 = vec4(pos.x + hLen, pos.y - hLen, pos.z + hLen, 1.0);
	pos4 = vec4(pos.x + hLen, pos.y - hLen, pos.z - hLen, 1.0);

	outColor = CalcFragColor(pos1, vec3(1.0, 0.0, 0.0));

	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos1; EmitVertex();
	EndPrimitive();

	gl_Position = mvp * pos4; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
	EndPrimitive();


	//bottom face
	pos1 = vec4(pos.x - hLen, pos.y - hLen, pos.z + hLen, 1.0);
	pos2 = vec4(pos.x + hLen, pos.y - hLen, pos.z + hLen, 1.0);
	pos3 = vec4(pos.x - hLen, pos.y - hLen, pos.z - hLen, 1.0);
	pos4 = vec4(pos.x + hLen, pos.y - hLen, pos.z - hLen, 1.0);

	outColor = CalcFragColor(pos1, vec3(0.0, -1.0, 0.0));

	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos1; EmitVertex();
	EndPrimitive();

	gl_Position = mvp * pos4; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos3; EmitVertex();
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
	EndPrimitive();

	gl_Position = mvp * pos3; EmitVertex();
	gl_Position = mvp * pos2; EmitVertex();
	gl_Position = mvp * pos4; EmitVertex();
	EndPrimitive();
}


void method3()
{	
	vec4 pos = gl_in[0].gl_Position;

	//front face
	vec4 pos1 = vec4(pos.x - hLen, pos.y + hLen, pos.z + hLen, 1.0);
	vec4 pos2 = vec4(pos.x + hLen, pos.y + hLen, pos.z + hLen, 1.0);
	vec4 pos3 = vec4(pos.x - hLen, pos.y - hLen, pos.z + hLen, 1.0);
	vec4 pos4 = vec4(pos.x + hLen, pos.y - hLen, pos.z + hLen, 1.0);

	gl_Position = mvp * pos1;  outColor = CalcFragColor(pos1, vec3(0.0, 0.0, 1.0));  EmitVertex();
	gl_Position = mvp * pos2;  outColor = CalcFragColor(pos2, vec3(0.0, 0.0, 1.0));  EmitVertex();
	gl_Position = mvp * pos3;  outColor = CalcFragColor(pos3, vec3(0.0, 0.0, 1.0));  EmitVertex();
	gl_Position = mvp * pos4;  outColor = CalcFragColor(pos4, vec3(0.0, 0.0, 1.0));  EmitVertex();

	EndPrimitive();

	
	//back face
	pos1 = vec4(pos.x - hLen, pos.y + hLen, pos.z - hLen, 1.0);
	pos2 = vec4(pos.x + hLen, pos.y + hLen, pos.z - hLen, 1.0);
	pos3 = vec4(pos.x - hLen, pos.y - hLen, pos.z - hLen, 1.0);
	pos4 = vec4(pos.x + hLen, pos.y - hLen, pos.z - hLen, 1.0);

	gl_Position = mvp * pos1;  outColor = CalcFragColor(pos1, vec3(0.0, 0.0, -1.0));  EmitVertex();
	gl_Position = mvp * pos2;  outColor = CalcFragColor(pos2, vec3(0.0, 0.0, -1.0));  EmitVertex();
	gl_Position = mvp * pos3;  outColor = CalcFragColor(pos3, vec3(0.0, 0.0, -1.0));  EmitVertex();
	gl_Position = mvp * pos4;  outColor = CalcFragColor(pos4, vec3(0.0, 0.0, -1.0));  EmitVertex();

	EndPrimitive();


	//left face
	pos1 = vec4(pos.x - hLen, pos.y + hLen, pos.z + hLen, 1.0);
	pos2 = vec4(pos.x - hLen, pos.y + hLen, pos.z - hLen, 1.0);
	pos3 = vec4(pos.x - hLen, pos.y - hLen, pos.z + hLen, 1.0);
	pos4 = vec4(pos.x - hLen, pos.y - hLen, pos.z - hLen, 1.0);

	gl_Position = mvp * pos1;  outColor = CalcFragColor(pos1, vec3(-1.0, 0.0, 0.0));  EmitVertex();
	gl_Position = mvp * pos2;  outColor = CalcFragColor(pos2, vec3(-1.0, 0.0, 0.0));  EmitVertex();
	gl_Position = mvp * pos3;  outColor = CalcFragColor(pos3, vec3(-1.0, 0.0, 0.0));  EmitVertex();
	gl_Position = mvp * pos4;  outColor = CalcFragColor(pos4, vec3(-1.0, 0.0, 0.0));  EmitVertex();

	EndPrimitive();
	

	//right face
	pos1 = vec4(pos.x + hLen, pos.y + hLen, pos.z + hLen, 1.0);
	pos2 = vec4(pos.x + hLen, pos.y + hLen, pos.z - hLen, 1.0);
	pos3 = vec4(pos.x + hLen, pos.y - hLen, pos.z + hLen, 1.0);
	pos4 = vec4(pos.x + hLen, pos.y - hLen, pos.z - hLen, 1.0);

	gl_Position = mvp * pos1;  outColor = CalcFragColor(pos1, vec3(1.0, 0.0, 0.0));  EmitVertex();
	gl_Position = mvp * pos2;  outColor = CalcFragColor(pos2, vec3(1.0, 0.0, 0.0));  EmitVertex();
	gl_Position = mvp * pos3;  outColor = CalcFragColor(pos3, vec3(1.0, 0.0, 0.0));  EmitVertex();
	gl_Position = mvp * pos4;  outColor = CalcFragColor(pos4, vec3(1.0, 0.0, 0.0));  EmitVertex();

	EndPrimitive();


	//bottom face
	pos1 = vec4(pos.x - hLen, pos.y - hLen, pos.z + hLen, 1.0);
	pos2 = vec4(pos.x + hLen, pos.y - hLen, pos.z + hLen, 1.0);
	pos3 = vec4(pos.x - hLen, pos.y - hLen, pos.z - hLen, 1.0);
	pos4 = vec4(pos.x + hLen, pos.y - hLen, pos.z - hLen, 1.0);

	gl_Position = mvp * pos1;  outColor = CalcFragColor(pos1, vec3(0.0, -1.0, 0.0));  EmitVertex();
	gl_Position = mvp * pos2;  outColor = CalcFragColor(pos2, vec3(0.0, -1.0, 0.0));  EmitVertex();
	gl_Position = mvp * pos3;  outColor = CalcFragColor(pos3, vec3(0.0, -1.0, 0.0));  EmitVertex();
	gl_Position = mvp * pos4;  outColor = CalcFragColor(pos4, vec3(0.0, -1.0, 0.0));  EmitVertex();

	EndPrimitive();


	//top face
	pos1 = vec4(pos.x - hLen, pos.y + hLen, pos.z + hLen, 1.0);
	pos2 = vec4(pos.x + hLen, pos.y + hLen, pos.z + hLen, 1.0);
	pos3 = vec4(pos.x - hLen, pos.y + hLen, pos.z - hLen, 1.0);
	pos4 = vec4(pos.x + hLen, pos.y + hLen, pos.z - hLen, 1.0);

	gl_Position = mvp * pos1;  outColor = CalcFragColor(pos1, vec3(0.0, 1.0, 0.0));  EmitVertex();
	gl_Position = mvp * pos2;  outColor = CalcFragColor(pos2, vec3(0.0, 1.0, 0.0));  EmitVertex();
	gl_Position = mvp * pos3;  outColor = CalcFragColor(pos3, vec3(0.0, 1.0, 0.0));  EmitVertex();
	gl_Position = mvp * pos4;  outColor = CalcFragColor(pos4, vec3(0.0, 1.0, 0.0));  EmitVertex();

	EndPrimitive();
}


void main(void)
{	
	if(methodNum == 1)
	{
		method1();
	}
	else if(methodNum == 2)
	{
		method2();
	}
	else if(methodNum == 3)
	{
		method3();
	}
}
