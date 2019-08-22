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

public:
	PhongCubeGeometryShader();
	~PhongCubeGeometryShader();

	void SetCubeHalfLen(float cubeHalfLen);
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif