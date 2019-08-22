#ifndef ColorShader_H
#define ColorShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

class ColorShader : public Shader
{
private:
	ShaderProgram* _shaderProgram;
	bool _use2DCam;

public:
	ColorShader();
	~ColorShader();

	void Set2DCamera(bool enable);
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif