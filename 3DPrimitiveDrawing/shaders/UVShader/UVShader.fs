#version 450

layout(location = 0) in vec2 uv_vary;
layout(location = 0) out vec4 outColor;

uniform sampler2D textureID;
uniform float alpha;
uniform bool alphaAsRGB;

void main (void)  
{
	outColor = texture2D(textureID, uv_vary);

	if(alphaAsRGB)
	{
		//outColor = vec4(outColor.a, outColor.a, outColor.a, 1.0);
	}
	else
	{
		outColor = vec4( outColor.rgb, alpha);
	}
}
