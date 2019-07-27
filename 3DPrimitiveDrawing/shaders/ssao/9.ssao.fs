#version 330 core
out float FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

// parameters (you'd probably want to use them as uniforms to more easily tweak the effect)
int kernelSize = 64;
float radius = 0.5;
float bias = 0.025;

// tile noise texture over screen based on screen dimensions divided by noise size
const vec2 noiseScale = vec2(1280.0/4.0, 720.0/4.0); 

uniform mat4 projection;


/*

float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float doAmbientOcclusion(vec2 tcoord, vec2 uv, vec3 p, vec3 cnorm)
{
    float scale = 0.5;
	float bias = 0.1;
	float intensity = 3;
	
    vec3 diff = vec3(texture(gPosition, tcoord + uv).xyz - p);
	
    vec3 v = normalize(diff);
    float d = length(diff) * scale;
    return max(0.0,dot(cnorm,v)-bias)*(1.0/(1.0+d))* intensity;
}

void main()
{
    vec2 texCoord = TexCoords; 
    vec3 p = texture(gPosition, texCoord.xy).xyz;	//vec3(10,10,-50); //
    vec3 n = texture(gNormal, texCoord.xy).xyz;		//vec3(0,-1,0);//
    vec2 rnd = normalize(vec2(rand(p.xy), rand(n.xy)));

    float ao = 0.0f;
    float rad = 1.0/p.z;
    vec2 vec[4]; 
    vec[0] = vec2(1.0,0.0); 
    vec[1] = vec2(-1.0,0.0); 
    vec[2] = vec2(0.0,1.0); 
    vec[3] = vec2(0.0,-1.0);

    int iterations = 2;
    for (int j = 0; j < iterations; ++j)
    {
      vec2 coord1 = reflect(vec[j],rnd)*rad;
	  
	  float xx = coord1.x*0.707;
	  float yy = coord1.y*0.707;
	  
      vec2 coord2 = vec2(xx-yy, xx+yy);
      
      ao += doAmbientOcclusion(texCoord.xy,coord1*0.25, p, n);
      ao += doAmbientOcclusion(texCoord.xy,coord2*0.5, p, n);
      ao += doAmbientOcclusion(texCoord.xy,coord1*0.75, p, n);
      ao += doAmbientOcclusion(texCoord.xy,coord2, p, n);
    }
    ao/= float(iterations)*4.0;
	float finalAO = 1.0 - ao;
	
    FragColor = finalAO;
}
*/


void main()
{
    // get input for SSAO algorithm
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);
    // create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);
    // iterate over the sample kernel and calculate occlusion factor
    float occlusion = 0.0;
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 sample = TBN * samples[i]; // from tangent to view-space
        sample = fragPos + sample * radius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(sample, 1.0);
        offset = projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        
        // get sample depth
        float sampleDepth = texture(gPosition, offset.xy).z; // get depth value of kernel sample
        
        // range check & accumulate
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;           
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    
    FragColor = occlusion;
}
