#version 450

uniform vec3 lightPos;
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

uniform float alpha;
uniform sampler2D textureID;

layout (location = 0) in vec3 V;
layout (location = 1) in vec3 N;
layout (location = 2) in vec2 uv_vary;

layout (location = 0) out vec4 outColor;

void main(void)
{
	vec3 L = normalize( lightPos - V );
	vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,N)); 
		
	vec4 Iamb = ambient;
	vec4 Idiff = diffuse * max(dot(N,L), 0.0);
	vec4 Ispec = specular * pow(max(dot(R,E),0.0), shininess*0.3);

	vec3 colorWitTex = vec3(Iamb + Idiff + Ispec) + texture2D(textureID, uv_vary).rgb;
	outColor = vec4( colorWitTex, alpha );
}
