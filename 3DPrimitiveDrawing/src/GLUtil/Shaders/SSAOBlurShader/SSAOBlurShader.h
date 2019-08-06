#ifndef SSAOBlurShader_H
#define SSAOBlurShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

class SSAOBlurShader : public Shader
{
private:
	float _alpha;
	GLMat _modelMat;
	unsigned int _ssaoInputTexID;

	ShaderProgram* _shaderProgram;

public:
	SSAOBlurShader();
	~SSAOBlurShader();

	void SetSSAOInputTexID(unsigned int texID);
	void SetModelMatrix(float* mat) override;
	void SetAlpha(float alpha) override;
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif