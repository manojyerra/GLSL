#version 450

uniform sampler2D textureID;
uniform float alpha;

layout(location = 0) in vec2 uv_vary;
layout(location = 0) out vec4 outColor;


void main (void)  
{
	outColor = vec4( texture2D(textureID, uv_vary).rgb, alpha );
}
