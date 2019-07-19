#ifndef ColorShader_H
#define ColorShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

class ColorShader : public Shader
{
private:
	unsigned int _vertexBufferID;
	unsigned int _colorBufferID;
	float _alpha;

	ShaderProgram* _shaderProgram;

	GLMat _modelMat;

public:
	ColorShader();
	~ColorShader();

	void SetVertexBufferID(unsigned int bufferID);
	void SetColorBufferID(unsigned int bufferID);

	void SetAlpha(float alpha) override;
	void SetModelMatrix(float* mat) override;
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif