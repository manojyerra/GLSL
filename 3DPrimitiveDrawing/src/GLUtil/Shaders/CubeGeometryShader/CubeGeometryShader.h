#ifndef CubeGeometryShader_H
#define CubeGeometryShader_H

#include "Shader.h"
#include "ShaderProgram.h"
#include "GLMat.h"

class CubeGeometryShader : public Shader
{
private:
	ShaderProgram* _shaderProgram;
	//int _methodNum;
	float _cubeHalfLen;
	float _alpha;
	GLMat _modelMat;

public:
	CubeGeometryShader();
	~CubeGeometryShader();

	void SetCubeHalfLen(float cubeHalfLen);
	void SetAlpha(float alpha) override;
	void SetModelMatrix(float* mat) override;
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif