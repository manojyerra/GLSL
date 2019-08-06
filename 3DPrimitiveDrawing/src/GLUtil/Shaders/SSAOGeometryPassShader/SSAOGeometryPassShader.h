#ifndef SSAOGeometryPassShader_H
#define SSAOGeometryPassShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

class SSAOGeometryPassShader : public Shader
{
private:
	ShaderProgram* _shaderProgram;

	float _alpha;
	GLMat _modelMat;

public:

	SSAOGeometryPassShader();
	~SSAOGeometryPassShader();

	void SetModelMatrix(float* mat);
	void SetAlpha(float alpha) override;
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif