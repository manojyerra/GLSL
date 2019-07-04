#version 450

layout (location = 0) in vec4 fragColor;

layout (location = 0) out vec4 outColor;

void main (void)  
{     
   outColor = fragColor;
}


//outColor = vec4(fragColor.r/255.0, fragColor.g/255.0, fragColor.b/255.0, fragColor.a/255.0);