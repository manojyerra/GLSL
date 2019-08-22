#ifndef CubeGeometryShader_H
#define CubeGeometryShader_H

#include "Shader.h"
#include "ShaderProgram.h"
#include "GLMat.h"

class CubeGeometryShader : public Shader
{
private:
	ShaderProgram* _shaderProgram;
	float _halfLen;

public:
	CubeGeometryShader();
	~CubeGeometryShader();

	void SetCubeHalfLen(float cubeHalfLen);
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif