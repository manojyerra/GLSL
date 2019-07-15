#ifndef GLMeshRenderer_H
#define GLMeshRenderer_H

#include "GLMat.h"
#include "ImageBuffer.h"
#include "ObjReader.h"
#include "BinaryObjReader.h"
#include "GLMeshBuilder.h"
#include "BasicShader.h"
#include "ColorShader.h"
#include "PhongShader.h"
#include "PBRShader.h"

class GLMeshRenderer
{
private:
	GLMeshBuilder* _meshBuilder;
	
	BasicShader* _basicShader;
	ColorShader* _colorShader;
	PhongShader* _phongPerVertexShader;
	PhongShader* _phongPerPixelShader;
	PBRShader* _pbrShader;

	int _shaderType;

	GLMat _modelMat;

	void CommonInit();

public:
	static const int BASIC_SHADER = 1;
	static const int COLOR_SHADER = 2;
	static const int PHONG_PER_VERTEX_SHADER = 3;
	static const int PHONG_PER_PIXEL_SHADER = 4;
	static const int PBR_SHADER = 5;

	GLMeshRenderer(ObjReader* reader);
	GLMeshRenderer(BinaryObjReader* reader);
	~GLMeshRenderer();

	void SetModelMatrix(float* mat);
	void SetPos(float x, float y, float z);
	void SetShader(int shaderType);
	void Draw();
};

#endif