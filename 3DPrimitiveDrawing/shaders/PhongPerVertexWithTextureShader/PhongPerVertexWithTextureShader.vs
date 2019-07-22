#version 450

uniform mat4 mvp;
uniform mat4 modelView;
uniform mat3 normalMat;

uniform vec3 lightPos;
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec2 uv_vary;


void CalcFragColor(vec3 V, vec3 N)
{
	vec3 L = normalize( lightPos - V );
	vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,N)); 
		
	vec4 Iamb = ambient;
	vec4 Idiff = diffuse * max(dot(N,L), 0.0);
	vec4 Ispec = specular * pow(max(dot(R,E),0.0), shininess*0.3);

	fragColor =  Iamb + Idiff; //+ Ispec;	
}

void main(void)
{
	vec3 V1 = vec3( modelView * vertex );
	vec3 N1 = normalize( normalMat * normal );
	
	CalcFragColor(V1, N1);
	uv_vary = uv;

	gl_Position = mvp * vertex;
}
