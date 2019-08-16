#ifndef QuadGeometryShader_H
#define QuadGeometryShader_H

#include "Shader.h"
#include "ShaderProgram.h"
#include "GLMat.h"

class QuadGeometryShader : public Shader
{
private:
	ShaderProgram* _shaderProgram;
	float _halfLen;
	float _alpha;
	GLMat _modelMat;

public:
	QuadGeometryShader();
	~QuadGeometryShader();

	void SetCubeHalfLen(float cubeHalfLen);
	void SetAlpha(float alpha) override;
	void SetModelMatrix(float* mat) override;
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif