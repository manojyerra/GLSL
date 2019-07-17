#ifndef UVShader_H
#define UVShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

class UVShader : public Shader
{
private:
	unsigned int _vertexBufferID;
	unsigned int _uvBufferID;
	unsigned int _textureID;
	float _alpha;

	ShaderProgram* _shaderProgram;

	GLMat _modelMat;
	bool _use2DCam;

public:
	UVShader();
	~UVShader();

	void SetVertexBufferID(unsigned int bufferID);
	void SetUVBufferID(unsigned int bufferID);
	void SetTextureID(unsigned int textureID);
	void SetModelMatrix(float* mat);
	void SetAlpha(float alpha);

	void Set2DCamera(bool enable);


	void Begin();
	void SetUniformsAndAttributes();
	void End();
};

#endif