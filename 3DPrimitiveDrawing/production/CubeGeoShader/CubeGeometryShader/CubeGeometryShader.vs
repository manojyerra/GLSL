#version 450

in vec3 vertex;
in vec3 normal;
in vec3 color;

uniform mat4 modelViewMat;
uniform mat3 normalMat;

layout (location = 0) out vec3 outColor;

vec3 CalcFragColor(vec3 col)
{
	vec3 V = vec3( modelViewMat * vec4(vertex,1.0) );
	vec3 N = normalize( normalMat * normal );
	vec3 L = normalize(-V);

	float dotNL = dot(N,L);
	
	if(dotNL < 0.0)
		dotNL = -dotNL;

	return col * max(dotNL, 0.0) + vec3(0.1,0.1,0.1);
}

void main(void)
{
	gl_Position = vec4(vertex,1.0);
	vec3 col = vec3(color.x/255.0, color.y/255.0, color.z/255.0);
	outColor = CalcFragColor(col);
}
