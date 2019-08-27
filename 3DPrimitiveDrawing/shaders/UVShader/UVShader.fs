#version 450

layout(location = 0) in vec2 uv_vary;
layout(location = 0) out vec4 outColor;

uniform sampler2D textureID;
uniform float alpha;
uniform bool alphaFromTex;

void main (void)  
{
	if(alphaFromTex)
	{
		outColor = texture2D(textureID, uv_vary);
	}
	else
	{
		outColor = vec4(texture2D(textureID, uv_vary).rgb, alpha);
	}
}
