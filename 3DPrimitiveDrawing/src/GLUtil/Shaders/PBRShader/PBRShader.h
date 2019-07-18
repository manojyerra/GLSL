#ifndef PBRShader_H
#define PBRShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

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
private:
	unsigned int _vertexBufferID;
	unsigned int _normalBufferID;

	std::map<string, LightInfo> _lightMap;
	
	glm::vec3 _albedo;
	float _metallic;
	float _roughness;
	float _alpha;

	ShaderProgram* _shaderProgram;
	int _shaderType;
	GLMat _modelMat;

public:
	static const int PER_PIXEL_SHADER = 1;
	static const int PER_VERTEX_SHADER = 2;

	PBRShader();
	~PBRShader();

	void SetVertexBufferID(unsigned int bufferID);
	void SetNormalBufferID(unsigned int bufferID);
	void SetModelMatrix(float* mat);
	void SetAlpha(float alpha);
	void SetAlbedo(glm::vec3& albedo);
	void SetMetallic(float metallic);
	void SetRoughness(float roughness);
	void SetMeterialProps(glm::vec3 albedo, float metallic, float roughtness);

	glm::vec3 GetAlbedo();
	float GetMetallic();
	float GetRoughness();
	std::map<string, LightInfo> GetLightMap();

	void AddLight(string lightName, LightInfo lightInfo);
	void RemoveLight(string lightName);

	void Begin();
	void SetUniformsAndAttributes();
	void End();
};

#endif