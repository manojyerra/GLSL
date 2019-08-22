#version 450

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

uniform mat4 modelViewMat;
uniform mat3 normalMat;

const float PI = 3.14159265359;
const int MAX_LIGHTS = 8;

layout (location = 0) in vec4 viewPosition;
layout (location = 1) in vec3 viewNormal;

uniform vec3 lightDir[MAX_LIGHTS];
uniform vec3 lightCol[MAX_LIGHTS];
uniform int numLights;

uniform float metallic;
uniform float roughness;
uniform float alpha;

layout (location = 0) out vec3 outColor;


float distributionGGX(float ndoth, float roughness)
{
	float roughness2 = roughness * roughness;
	float roughness4 = roughness2 * roughness2;
	float ndoth2 = ndoth * ndoth;
	float denominator = (ndoth2 * (roughness4 - 1.0) + 1.0);
	denominator = PI * denominator * denominator;
	return roughness4 / denominator;
}

float geometrySchlickGGX(float cosTheta, float roughness)
{
	float r = (roughness + 1);
	float k = (r * r) / 8.0;
	float denominator = cosTheta * (1.0 - k) + k;
	return cosTheta / denominator;
}

float geometrySmith(float ndotv, float ndotl, float roughness)
{
	float ggx1 = geometrySchlickGGX(ndotv, roughness);
	float ggx2 = geometrySchlickGGX(ndotl, roughness);
	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 f0)
{
	return f0 + (1.0 - f0) * pow(1.0 - cosTheta, 5.0);
}

void main(void)
{
	gl_Position = vertex;
	vec3 viewNormal = normalMat * normal;
	vec4 viewPosition = modelViewMat * vertex;
	vec3 viewColor = vec3(color.x/255.0, color.y/255.0, color.z/255.0);
	
	vec3 lo = vec3(0.0);

	for(int i=0; i<numLights && i<MAX_LIGHTS; i++)
	{
		vec4 dir = vec4(lightDir[i], 1.0);
		vec4 col = vec4(lightCol[i], 1.0);
		
		dir = normalize(dir);

		vec3 normal = normalize(viewNormal);
		vec3 viewDir = normalize(-viewPosition.xyz);
		vec3 f0 = vec3(0.04);
		vec3 albedo = viewColor;
		f0 = mix(f0, albedo, metallic);

		vec3 lightDir = normalize(-dir.xyz);
		vec3 halfDir = normalize(lightDir + viewDir);
		float hdotv = max(abs(dot(halfDir, viewDir)), 0.0);
		float ndotl = max(abs(dot(normal, lightDir)), 0.0);
		float ndotv = max(abs(dot(normal, viewDir)), 0.0);
		float ndoth = max(abs(dot(normal, halfDir)), 0.0);

		float ndf = distributionGGX(ndoth, roughness);
		float g = geometrySmith(ndotv, ndotl, roughness);
		vec3 f = fresnelSchlick(hdotv, f0);

		vec3 kS = f;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - metallic;

		vec3 nominator = ndf * g * f;
		float denominator = 4 * ndotv * ndotl + 0.00001;
		vec3 specular = nominator / denominator;

		lo += (kD * albedo / PI + specular) * col.rgb * ndotl;
	}

	vec3 ambient = vec3(0.03);
	vec3 finalColor = lo + ambient;

	finalColor = finalColor / (finalColor + vec3(1.0));
    finalColor = pow(finalColor, vec3(1.0/2.2));  

	outColor = finalColor;
}
