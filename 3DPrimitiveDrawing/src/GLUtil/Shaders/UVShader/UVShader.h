#ifndef UVShader_H
#define UVShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

class UVShader : public Shader
{
private:
	ShaderProgram* _shaderProgram;
	bool _use2DCam;

public:
	UVShader();
	~UVShader();

	void Set2DCamera(bool enable);
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif