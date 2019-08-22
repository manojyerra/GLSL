#ifndef PhongWithTextureShader_H
#define PhongWithTextureShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

class PhongWithTextureShader : public Shader
{
private:
	glm::vec3 lightPos;
	float _Ka[4];
	float _Kd[4];
	float _Ks[4];
	float _Se;

	ShaderProgram* _perPixelShader;
	ShaderProgram* _perVertexShader;
	ShaderProgram* _shaderProgram;

	int _shaderType;

public:
	static const int PER_PIXEL_SHADER = 1;
	static const int PER_VERTEX_SHADER = 2;

	PhongWithTextureShader(int shaderType);
	~PhongWithTextureShader();

	void SetShaderType(int shaderType);

	void SetLightPos(float x, float y, float z);
	void SetAmbientColor(float r, float g, float b, float a);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetSpecularColor(float r, float g, float b, float a);
	void SetShininess(float shininess);

	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif