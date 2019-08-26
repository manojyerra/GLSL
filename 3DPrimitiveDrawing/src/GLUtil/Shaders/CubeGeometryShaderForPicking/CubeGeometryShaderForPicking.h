#ifndef CubeGeometryShaderForPicking_H
#define CubeGeometryShaderForPicking_H

#include "Shader.h"
#include "ShaderProgram.h"
#include "GLMat.h"

class CubeGeometryShaderForPicking : public Shader
{
private:
	ShaderProgram* _shaderProgram;
	float _halfLen;

public:
	CubeGeometryShaderForPicking();
	~CubeGeometryShaderForPicking();

	void SetCubeHalfLen(float cubeHalfLen);
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif