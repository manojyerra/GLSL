#ifndef SSAOGeometryPassShader_H
#define SSAOGeometryPassShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

class SSAOGeometryPassShader : public Shader
{
private:
	ShaderProgram* _shaderProgram;

public:

	SSAOGeometryPassShader();
	~SSAOGeometryPassShader();

	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif