#version 450

layout(location = 0) in vec2 uv_vary;
layout(location = 0) out vec4 outColor;

uniform sampler2D textureID;
uniform float alpha;

void main (void)  
{
	outColor = vec4( texture2D(textureID, uv_vary).rgb, alpha);
}
