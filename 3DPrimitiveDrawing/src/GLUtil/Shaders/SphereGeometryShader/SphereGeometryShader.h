#ifndef SphereGeometryShader_H
#define SphereGeometryShader_H

#include "Shader.h"
#include "ShaderProgram.h"
#include "GLMat.h"

class SphereGeometryShader : public Shader
{
private:
	ShaderProgram* _shaderProgram;
	float _halfLen;

public:
	SphereGeometryShader();
	~SphereGeometryShader();

	void SetCubeHalfLen(float cubeHalfLen);
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif