#ifndef SSAOBlurShader_H
#define SSAOBlurShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

class SSAOBlurShader : public Shader
{
private:
	unsigned int _ssaoInputTexID;

	ShaderProgram* _shaderProgram;

public:
	SSAOBlurShader();
	~SSAOBlurShader();

	void SetSSAOInputTexID(unsigned int texID);
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif