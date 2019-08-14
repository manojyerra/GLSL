#ifndef PhongColorShader_H
#define PhongColorShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

class PhongColorShader : public Shader
{
private:
	glm::vec3 lightPos;
	float _Ka[4];
	float _Kd[4];
	float _Ks[4];
	float _Se;

	float _alpha;

	ShaderProgram* _shaderProgram;

	GLMat _modelMat;

public:
	PhongColorShader();
	~PhongColorShader();

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