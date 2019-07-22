#version 450

uniform float alpha;

layout(location = 0) in vec4 fragColor;

layout(location = 0) out vec4 outColor;


void main (void)  
{     
	outColor = vec4(fragColor.rgb, alpha);
}
