#version 450

layout (location = 0) out vec4 outColor;

uniform vec3 color;
uniform float alpha;

void main (void)
{
   outColor = vec4(color, alpha);
}
