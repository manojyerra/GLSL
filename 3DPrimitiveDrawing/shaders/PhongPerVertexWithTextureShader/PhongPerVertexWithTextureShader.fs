#version 450

uniform sampler2D textureID;
uniform float alpha;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 uv_vary;

layout(location = 0) out vec4 outColor;


void main (void)  
{     
	vec3 colorWithTex = fragColor.rgb + texture2D(textureID, uv_vary).rgb;
	outColor = vec4(colorWithTex.rgb, alpha);
}
