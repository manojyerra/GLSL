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

public:
	QuadGeometryShader();
	~QuadGeometryShader();

	void SetCubeHalfLen(float cubeHalfLen);
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif