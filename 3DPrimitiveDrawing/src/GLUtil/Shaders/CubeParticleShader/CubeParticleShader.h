#ifndef CubeParticleShader_H
#define CubeParticleShader_H

#include "Shader.h"
#include "ShaderProgram.h"
#include "GLMat.h"

class CubeParticleShader : Shader
{
private:
	ShaderProgram* _shaderProgram;
	unsigned int _vertexBufferID;
	unsigned int _colorBufferID; 

	int _methodNum;
	float _cubeHalfLen;
	float _alpha;
	GLMat _modelMat;

public:
	CubeParticleShader();
	~CubeParticleShader();

	void SetCubeHalfLen(float cubeHalfLen);
	void SetVertexBufferID(unsigned int bufferID);
	void SetColorBufferID(unsigned int bufferID);

	void SetAlpha(float alpha) override;
	void SetModelMatrix(float* mat) override;
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif