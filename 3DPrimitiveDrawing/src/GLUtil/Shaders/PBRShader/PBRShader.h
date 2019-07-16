#ifndef PBRShader_H
#define PBRShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

#include <vector>
#include <string>
using namespace std;


class PBRShader : public Shader
{
private:
	unsigned int _vertexBufferID;
	unsigned int _normalBufferID;

	float _alpha;
	glm::vec3 _lightDir;
	glm::vec3 _lightColor;
	glm::vec3 _albedo;
	float _metallic;
	float _roughness;

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
	void SetLightDirection(glm::vec3& lightDir);
	void SetLightColor(glm::vec3& lightColor);
	void SetAlbedo(glm::vec3& albedo);
	void SetMetallic(float metallic);
	void SetRoughness(float roughness);

	void Begin();
	void SetUniformsAndAttributes();
	void End();
};

#endif