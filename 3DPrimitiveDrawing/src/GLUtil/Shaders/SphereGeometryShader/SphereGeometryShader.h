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
	float _alpha;
	GLMat _modelMat;

public:
	SphereGeometryShader();
	~SphereGeometryShader();

	void SetCubeHalfLen(float cubeHalfLen);
	void SetAlpha(float alpha) override;
	void SetModelMatrix(float* mat) override;
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif