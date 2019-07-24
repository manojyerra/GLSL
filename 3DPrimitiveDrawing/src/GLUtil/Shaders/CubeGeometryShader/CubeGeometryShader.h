#ifndef CubeGeometryShader_H
#define CubeGeometryShader_H

#include "Shader.h"
#include "ShaderProgram.h"
#include "GLMat.h"

class CubeGeometryShader : public Shader
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
	CubeGeometryShader();
	~CubeGeometryShader();

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