#version 450

uniform sampler2D textureID;
uniform float alpha;
uniform float occlusionLevel;

layout(location = 0) in vec2 uvVary;
layout(location = 0) out vec4 outColor;


void main (void)  
{
	float r = texture2D(textureID, uvVary).r;
	
	outColor = vec4( 0.0, 0.0, 0.0, (1.0-r)*occlusionLevel);
}
