#ifndef GLTexture_H
#define GLTexture_H

#include "GLMat.h"
#include "GLMeshRenderer.h"
#include "UVShader.h"

class GLTexture
{
private:
	unsigned int _textureID;
	GLMat _modelMat;
	GLMeshRenderer* _meshRenderer;

	void GenerateGeometry(const char* texturePath);

public:
	GLTexture(const char* texturePath, float x, float y, float w, float h);
	GLTexture(float x, float y, float w, float h);

	~GLTexture();

	UVShader* GetShader();
	void SetBounds(float x, float y, float w, float h);
	void Draw();
};

#endif
