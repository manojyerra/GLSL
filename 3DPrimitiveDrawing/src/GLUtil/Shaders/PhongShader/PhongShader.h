#ifndef PhongShader_H
#define PhongShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

#include <vector>
#include <string>
using namespace std;


class PhongShader : public Shader
{
private:
	unsigned int _vertexBufferID;
	unsigned int _normalBufferID;
	unsigned int _uvBufferID;
	unsigned int _baseTexID;

	glm::vec3 lightPos;
	float _Ka[4];
	float _Kd[4];
	float _Ks[4];
	float _Se;

	float _alpha;

	ShaderProgram* _perPixelShader;
	ShaderProgram* _perVertexShader;
	ShaderProgram* _shaderProgram;

	int _shaderType;

	GLMat _modelMat;

public:
	static const int PER_PIXEL_SHADER = 1;
	static const int PER_VERTEX_SHADER = 2;

	PhongShader(int shaderType);
	~PhongShader();

	void SetShaderType(int shaderType);

	void SetVertexBufferID(unsigned int bufferID);
	void SetNormalBufferID(unsigned int bufferID);
	void SetUVBufferID(unsigned int bufferID);
	void SetBaseTexID(unsigned int texID);
	void SetModelMatrix(float* mat);

	void SetLightPos(float x, float y, float z);
	void SetAmbientColor(float r, float g, float b, float a);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetSpecularColor(float r, float g, float b, float a);
	void SetShininess(float shininess);

	void SetAlpha(float alpha) override;
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif