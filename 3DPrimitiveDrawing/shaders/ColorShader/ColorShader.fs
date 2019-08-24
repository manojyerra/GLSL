#version 450

layout (location = 0) in vec3 color_vary;
layout (location = 0) out vec4 outColor;

uniform float alpha;

void main (void)  
{     
   outColor = vec4(color_vary, alpha);
}