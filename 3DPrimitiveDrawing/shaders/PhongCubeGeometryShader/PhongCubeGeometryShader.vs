#version 450

in vec4 vertex;
in vec3 normal;
in vec3 color;

layout (location = 0) out vec3 outColor;

uniform mat4 modelViewMat;
uniform mat3 normalMat;

vec3 CalcFragColor(vec4 diffuse)
{
	vec3 V = vec3( modelViewMat * vertex );
	vec3 N = normalize( normalMat * normal );	

	vec3 L = normalize(-V);
	vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,N)); 

	vec4 ambient = vec4(0.05375, 0.05, 0.06625,	1.0);
	vec4 specular = vec4(0.33274, 0.32,	0.34643, 1.0);
	float shininess = 38.4;

	vec4 Iamb = ambient;
	vec4 Idiff = diffuse * max(abs(dot(R,E)), 0.3);
	vec4 Ispec = specular * pow(max(dot(R,E),0.3), shininess*0.3);

	return vec3(Iamb + Idiff + Ispec);
}

void main(void)
{
	vec4 diffuse = vec4(color.x/255.0, color.y/255.0, color.z/255.0, 1.0);
	outColor = CalcFragColor(diffuse);
	
	gl_Position = vertex;	
}
