#version 450

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec3 inPos;
layout (location = 2) in vec3 inCenPos;

layout (location = 0) out vec4 outColor;

uniform float radiusPow2;

void main(void)
{
	vec3 delta = inCenPos - inPos;
	float dist = delta.x*delta.x + delta.y*delta.y + delta.z*delta.z;

	if(dist > radiusPow2)
	{
		discard;
	}
	else
	{
		float factor = 1.0 - dist / radiusPow2;
		outColor = vec4(inColor*factor, 1.0);
	}
}
