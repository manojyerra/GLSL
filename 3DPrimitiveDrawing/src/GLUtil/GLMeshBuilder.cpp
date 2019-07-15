#include "GLMeshBuilder.h"
#include "GLMemory.h"

GLMeshBuilder::GLMeshBuilder()
{
	_vertexBufferID = 0;
	_normalBufferID = 0;
	_uvBufferID = 0;
	_baseTexID = 0;

	_vertexArr = NULL;
	_normalBufferID = NULL;
	_uvBufferID = NULL;
	_imageBuffer = NULL;

	_vertexArrLen = 0;
	_normalArrLen = 0;
	_uvArrLen = 0;
}

GLMeshBuilder* GLMeshBuilder::SetVertexArray(const GLvoid* arr, GLsizeiptr len)
{
	_vertexArr = arr;
	_vertexArrLen = len;
	return this;
}

GLMeshBuilder* GLMeshBuilder::SetNormalArray(const GLvoid* arr, GLsizeiptr len)
{
	_normalArr = arr;
	_normalArrLen = len;
	return this;
}

GLMeshBuilder* GLMeshBuilder::SetUVArray(const GLvoid* arr, GLsizeiptr len)
{
	_uvArr = arr;
	_uvArrLen = len;
	return this;
}

GLMeshBuilder* GLMeshBuilder::SetImageBuffer(ImageBuffer* imgBuf)
{
	_imageBuffer = imgBuf;
	return this;
}

bool GLMeshBuilder::build()
{
	if(_vertexArr)
	{
		_vertexBufferID = GLCreateBuffer(_vertexArrLen, (GLvoid*)_vertexArr);
	}

	if (_normalArr)
	{
		_normalBufferID = GLCreateBuffer(_normalArrLen, (GLvoid*)_normalArr);
	}

	if (_uvArr)
	{
		_uvBufferID = GLCreateBuffer(_uvArrLen, (GLvoid*)_uvArr);
	}

	if (_imageBuffer)
	{
		GLsizei width = _imageBuffer->GetWidth();
		GLsizei height = _imageBuffer->GetHeight();
		unsigned int bytesPP = _imageBuffer->GetBytesPerPixel();
		GLvoid* buffer = _imageBuffer->GetBuffer();

		_baseTexID = GLCreateTexture(width, height, bytesPP, buffer);
	}
}

GLMeshBuilder::~GLMeshBuilder()
{
	if (_vertexArr)
	{
		GLDeleteBuffer(_vertexBufferID);
		_vertexBufferID = 0;
	}

	if (_normalArr)
	{
		GLDeleteBuffer(_normalBufferID);
		_normalBufferID = 0;
	}

	if (_uvArr)
	{
		GLDeleteBuffer(_uvBufferID);
		_uvBufferID = 0;
	}

	if (_imageBuffer)
	{
		GLDeleteTexture(_baseTexID);
		_baseTexID = 0;
	}
}
