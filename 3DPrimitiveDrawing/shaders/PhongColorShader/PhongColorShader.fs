#version 450

uniform vec3 lightPos;
uniform vec4 ambient;
uniform vec4 specular;
uniform float shininess;

uniform float alpha;

layout (location = 0) in vec3 V;
layout (location = 1) in vec3 N;
layout (location = 2) in vec3 colorVary;

layout (location = 0) out vec4 outColor;

void main(void)
{
	vec3 L = normalize( lightPos - V );
	vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,N)); 

	vec4 diffuse = vec4(colorVary, 1.0);
	vec4 Iamb = ambient;
	vec4 Idiff = diffuse * max(abs(dot(N,L)), 0.0);
	vec4 Ispec = specular * pow(max(dot(R,E),0.0), shininess*0.3);

	outColor = vec4( vec3(Iamb + Idiff + Ispec), alpha );
}
