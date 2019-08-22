#ifndef PhongCubeGeometryShader_H
#define PhongCubeGeometryShader_H

#include "Shader.h"
#include "ShaderProgram.h"
#include "GLMat.h"

class PhongCubeGeometryShader : public Shader
{
private:
	ShaderProgram* _shaderProgram;
	float _halfLen;
	float _alpha;
	GLMat _modelMat;

public:
	PhongCubeGeometryShader();
	~PhongCubeGeometryShader();

	void SetCubeHalfLen(float cubeHalfLen);
	void SetAlpha(float alpha) override;
	void SetModelMatrix(float* mat) override;
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif