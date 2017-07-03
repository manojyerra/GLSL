#version 120

attribute vec4 vertex;
attribute vec3 normal;

uniform vec3 lightPos;

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float shininess;

varying vec4 fragColor;


void main(void)
{
	vec3 V = vec3( gl_ModelViewMatrix * vertex );
	vec3 N = normalize( gl_NormalMatrix * normal );

	vec3 L = normalize( lightPos - V );
	vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,N)); 
		
	vec4 Iamb = ambient;
	vec4 Idiff = diffuse * max(dot(N,L), 0.0);
	vec4 Ispec = specular * pow(max(dot(R,E),0.0), shininess*0.3);

	fragColor = Iamb + Idiff + Ispec;

	gl_Position = gl_ModelViewProjectionMatrix * vertex;
}
