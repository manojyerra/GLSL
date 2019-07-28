#version 450

//out vec4 outColor;
out float outColor;

in vec2 uvVary;

uniform sampler2D ssaoInput;

void main() 
{
    vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
	
    float result = 0.0;
	int len = 2;
	
    for (int x = -len; x < len; ++x) 
    {
        for (int y = -len; y < len; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssaoInput, uvVary + offset).r;
        }
    }
	
    result = result / (4.0 * 4.0);
	
	//outColor = vec4(result, result, result, 1.0);
	//outColor = (result > 1.0 ? 1.0 : result);
	
	outColor = result;
}  