#ifndef GLMeshRenderer_H
#define GLMeshRenderer_H

#include "GLMat.h"
#include "ImageBuffer.h"
#include "ModelInfo.h"
#include "GLMeshBuilder.h"
#include "BasicShader.h"
#include "ColorShader.h"
#include "UVShader.h"
#include "PhongShader.h"
#include "PhongWithTextureShader.h"
#include "PBRShader.h"
#include "PBRWithTextureShader.h"


class GLMeshRenderer
{
private:
	GLMeshBuilder* _meshBuilder;
	
	BasicShader* _basicShader;
	ColorShader* _colorShader;
	UVShader* _uvShader;
	PhongShader* _phongPerVertexShader;
	PhongShader* _phongPerPixelShader;
	PhongWithTextureShader* _phongWithTexPerVertexShader;
	PhongWithTextureShader* _phongWithTexPerPixelShader;
	PBRShader* _pbrShader;
	PBRWithTextureShader* _pbrWithTextureShader;

	Shader* _shader;

	int _shaderType;
	int _primitiveType;

	GLMat _modelMat;

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

	enum {
		BASIC_SHADER,
		COLOR_SHADER,
		UV_SHADER,
		PHONG_PER_VERTEX_SHADER,
		PHONG_PER_PIXEL_SHADER,
		PHONG_WITH_TEXTURE_PER_VERTEX_SHADER,
		PHONG_WITH_TEXTURE_PER_PIXEL_SHADER,
		PBR_SHADER,
		PBR_WITH_TEXTURE_SHADER
	};

	GLMeshRenderer(ModelInfo* meshCreateInfo, int shaderType);
	~GLMeshRenderer();

	void SetModelMatrix(float* mat);
	void SetPos(float x, float y, float z);
	glm::vec3 GetPos();

	void SetShader(int shaderType);
	Shader* GetShader(int shaderType);
	Shader* GetCurrentShader();

	void SetPrimitiveType(int primitiveType);
	void SetAlpha(float alpha);

	void Draw();
};

#endif