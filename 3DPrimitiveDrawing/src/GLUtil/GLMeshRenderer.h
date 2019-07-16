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


class GLMeshCreateInfo
{
private:
	const char* _vertexBuf;
	const char* _uvBuf;
	const char* _normalBuf;
	const char* _colorBuf;
	ImageBuffer* _imageBuf;

	unsigned int _vertexBufLen;
	unsigned int  _uvBufLen;
	unsigned int _normalBufLen;
	unsigned int _colorBufLen;

public:
	GLMeshCreateInfo()
	{
		_vertexBuf = NULL;
		_uvBuf = NULL;
		_normalBuf = NULL;
		_colorBuf = NULL;
		_imageBuf = NULL;

		_vertexBufLen = 0;
		_uvBufLen = 0;
		_normalBufLen = 0;
		_colorBufLen = 0;
	}

	void SetVertexBuffer(const char* buf, unsigned int len)
	{
		_vertexBuf = buf;
		_vertexBufLen = len;
	}

	void SetUVBuffer(const char* buf, unsigned int len)
	{
		_uvBuf = buf;
		_uvBufLen = len;
	}

	void SetNormalBuffer(const char* buf, unsigned int len)
	{
		_normalBuf = buf;
		_normalBufLen = len;
	}

	void SetColorBuffer(const char* buf, unsigned int len)
	{
		_colorBuf = buf;
		_colorBufLen = len;
	}

	void SetImageBufferInfo(ImageBuffer* imageBuf)
	{
		_imageBuf = imageBuf;
	}

	const char* GetVertexBuffer()	{	return _vertexBuf;	}
	const char* GetUVBuffer()		{	return _uvBuf;		}
	const char* GetNormalBuffer()	{	return _normalBuf;	}
	const char* GetColorBuffer()	{	return _colorBuf;	}
	ImageBuffer* GetImageBuffer()	{	return _imageBuf;	}

	unsigned int GetVertexBufferSize()	{ return _vertexBufLen; }
	unsigned int GetUVBufferSize()		{ return _uvBufLen;		}
	unsigned int GetNormalBufferSize()	{ return _normalBufLen; }
	unsigned int GetColorBufferSize()	{ return _colorBufLen;	}

	~GLMeshCreateInfo()
	{
	}
};

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

	GLMeshRenderer(ObjReader* reader);
	GLMeshRenderer(BinaryObjReader* reader);
	GLMeshRenderer(GLMeshCreateInfo* meshCreateInfo);

	~GLMeshRenderer();

	void SetModelMatrix(float* mat);
	void SetPos(float x, float y, float z);
	void SetShader(int shaderType);
	void SetPrimitiveType(int primitiveType);
	void Draw();
};

#endif