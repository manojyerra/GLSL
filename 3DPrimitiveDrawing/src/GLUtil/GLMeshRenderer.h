#ifndef GLMeshRenderer_H
#define GLMeshRenderer_H

#include "GLMat.h"
#include "ImageBuffer.h"
#include "ModelCreateInfo.h"
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
	Shader* _shader;

	int _shaderType;
	int _primitiveType;

	GLMat _modelMat;

	void CommonInit();

public:
	enum {
		points = GL_POINTS,
		lines = GL_LINES,
		lineLoop = GL_LINE_LOOP,
		lineStrip = GL_LINE_STRIP,
		triangles = GL_TRIANGLES,
		triangleStrip = GL_TRIANGLE_STRIP,
		triangleFan = GL_TRIANGLE_FAN
	};

	static const int BASIC_SHADER = 1;
	static const int COLOR_SHADER = 2;
	static const int PHONG_PER_VERTEX_SHADER = 3;
	static const int PHONG_PER_PIXEL_SHADER = 4;
	static const int PBR_SHADER = 5;

	GLMeshRenderer(ModelCreateInfo* meshCreateInfo);

	~GLMeshRenderer();

	void SetModelMatrix(float* mat);
	void SetPos(float x, float y, float z);
	void SetShader(int shaderType);
	void SetPrimitiveType(int primitiveType);
	void Draw();
};

#endif