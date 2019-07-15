#ifndef GLMeshRenderer_H
#define GLMeshRenderer_H

#include "GLMat.h"
#include "ImageBuffer.h"
#include "ObjReader.h"
#include "BinaryObjReader.h"
#include "GLMeshBuilder.h"
#include "BasicShader.h"

class GLMeshRenderer
{
private:
	GLMeshBuilder* _meshBuilder;
	BasicShader* _basicShader;
	int _shaderType;

	void CommonInit();

public:
	static const int BASIC_SHADER = 1;
	static const int COLOR_SHADER = 2;

	GLMeshRenderer(ObjReader* reader);
	GLMeshRenderer(BinaryObjReader* reader);
	~GLMeshRenderer();

	void SetShader(int shaderType);
	void Draw();
};

#endif