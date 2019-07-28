#version 450

out vec4 outColor;

in vec2 uvVary;

uniform sampler2D ssaoInput;

void main() 
{
    vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
    float result = 0.0;
	
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssaoInput, uvVary + offset).r;
        }
    }
	
    outColor = result / (4.0 * 4.0);
	
	outColor = vec4(outColor, outColor, outColor, 1.0);
}  