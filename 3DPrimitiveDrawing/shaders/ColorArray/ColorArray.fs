#version 450

uniform float alpha;

layout (location = 0) in vec3 color_vary;
layout (location = 0) out vec4 outColor;

void main (void)  
{     
   outColor = vec4(color_vary, alpha);
}