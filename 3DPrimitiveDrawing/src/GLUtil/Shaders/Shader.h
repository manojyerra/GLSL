#ifndef Shader_H
#define Shader_H

#include "GLMat.h"

class Shader 
{
protected:
	unsigned int _vertexBufferID;
	unsigned int _colorBufferID;
	unsigned int _normalBufferID;
	unsigned int _uvBufferID;
	unsigned int _textureID;

	float _alpha;
	GLMat _modelMat;

public:
	Shader()
	{
		_vertexBufferID = 0;
		_colorBufferID = 0;
		_normalBufferID = 0;
		_uvBufferID = 0;
		_textureID = 0;
		_alpha = 1.0f;
	}

	virtual ~Shader()
	{
	}

	void SetAlpha(float alpha)
	{
		_alpha = alpha;
	}

	void SetModelMatrix(float* mat)
	{
		_modelMat.Copy(mat);
	}

	virtual void Begin() = 0;
	virtual void SetUniformsAndAttributes() = 0;
	virtual void End() = 0;

	void SetVertexBufferID(unsigned int bufferID)
	{
		_vertexBufferID = bufferID;
	}

	void SetColorBufferID(unsigned int bufferID)
	{
		_colorBufferID = bufferID;
	}

	void SetNormalBufferID(unsigned int bufferID)
	{
		_normalBufferID = bufferID;
	}

	void SetUVBufferID(unsigned int bufferID)
	{
		_uvBufferID = bufferID;
	}

	void SetTextureID(unsigned int textureID)
	{
		_textureID = textureID;
	}
};

#endif