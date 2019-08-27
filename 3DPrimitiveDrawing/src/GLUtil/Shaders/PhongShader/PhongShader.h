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
	glm::vec3 lightPos;
	float _Ka[4];
	float _Kd[4];
	float _Ks[4];
	float _Se;
	bool _invertNormal;

	ShaderProgram* _perPixelShader;
	ShaderProgram* _perVertexShader;
	ShaderProgram* _shaderProgram;

	int _shaderType;

public:
	static const int PER_PIXEL_SHADER = 1;
	static const int PER_VERTEX_SHADER = 2;

	PhongShader(int shaderType);
	~PhongShader();

	void SetShaderType(int shaderType);

	void SetLightPos(float x, float y, float z);
	void SetAmbientColor(float r, float g, float b, float a);
	void SetDiffuseColor(float r, float g, float b, float a);
	void SetSpecularColor(float r, float g, float b, float a);
	void SetShininess(float shininess);
	void SetInvertNormal(bool invertNormal);

	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif