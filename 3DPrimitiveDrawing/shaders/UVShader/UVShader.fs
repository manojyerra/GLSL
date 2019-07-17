#version 450

uniform sampler2D textureID;
uniform float alpha;

layout(location = 0) in vec2 uv_varying;
layout(location = 0) out vec4 outColor;


void main (void)  
{
	outColor = vec4( texture2D(textureID, uv_varying).rgb, alpha );
}
