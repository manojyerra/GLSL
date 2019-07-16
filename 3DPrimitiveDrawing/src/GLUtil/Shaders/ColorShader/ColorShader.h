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
	void SetModelMatrix(float* mat);

	void SetAlpha(float alpha);

	void Begin();
	void SetUniformsAndAttributes();
	void End();
};

#endif