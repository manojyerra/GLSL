#ifndef PBRShader_H
#define PBRShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"
#include <map>

#include <vector>
#include <string>

class LightInfo
{
public:
	glm::vec3 dir;
	glm::vec3 color;

	LightInfo(glm::vec3 dir, glm::vec3 color)
	{
		this->dir = dir;
		this->color = color;
	}

	bool operator< (const LightInfo& obj) const
	{
		return (obj.dir.x < this->dir.x);
	}
};

class PBRShader : public Shader
{
protected:
	std::map<std::string, LightInfo> _lightMap;
	
	glm::vec3 _albedo;
	float _metallic;
	float _roughness;

	ShaderProgram* _shaderProgram;
	int _shaderType;

public:
	PBRShader();
	PBRShader(std::string vertexShaderPath, std::string fragmentShaderPath);
	virtual ~PBRShader();

	void CommonInit(std::string vertexShaderPath, std::string fragmentShaderPath);
	void SetAlbedo(glm::vec3& albedo);
	void SetMetallic(float metallic);
	void SetRoughness(float roughness);
	void SetMeterialProps(glm::vec3 albedo, float metallic, float roughtness);

	glm::vec3 GetAlbedo();
	float GetMetallic();
	float GetRoughness();
	std::map<std::string, LightInfo> GetLightMap();

	void AddLight(std::string lightName, LightInfo lightInfo);
	void RemoveLight(std::string lightName);

	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif