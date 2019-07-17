#include "GLMeshBuilder.h"
#include "GLMemory.h"

GLMeshBuilder::GLMeshBuilder()
{
	_vertexBufferID = 0;
	_normalBufferID = 0;
	_uvBufferID = 0;
	_colorBufferID = 0;
	_indicesBufferID = 0;
	_baseTexID = 0;

	_vertexBuffer = NULL;
	_normalBufferID = NULL;
	_uvBufferID = NULL;
	_colorBufferID = NULL;
	_indicesBuffer = NULL;
	_imageBuffer = NULL;

	_vertexBufferLen = 0;
	_normalBufferLen = 0;
	_uvBufferLen = 0;
	_colorBufferLen = 0;
	_indicesBufferLen = 0;
}

GLMeshBuilder* GLMeshBuilder::SetVertexBuffer(const GLvoid* buffer, GLsizeiptr len)
{
	_vertexBuffer = buffer;
	_vertexBufferLen = len;
	return this;
}

GLMeshBuilder* GLMeshBuilder::SetNormalBuffer(const GLvoid* buffer, GLsizeiptr len)
{
	_normalBuffer = buffer;
	_normalBufferLen = len;
	return this;
}

GLMeshBuilder* GLMeshBuilder::SetUVBuffer(const GLvoid* buffer, GLsizeiptr len)
{
	_uvBuffer = buffer;
	_uvBufferLen = len;
	return this;
}

GLMeshBuilder* GLMeshBuilder::SetColorBuffer(const GLvoid* buffer, GLsizeiptr len)
{
	_colorBuffer = buffer;
	_colorBufferLen = len;
	return this;
}

GLMeshBuilder* GLMeshBuilder::SetIndicesBuffer(const GLvoid* buffer, GLsizeiptr len)
{
	_indicesBuffer = buffer;
	_indicesBufferLen = len;
	return this;
}

GLMeshBuilder* GLMeshBuilder::SetImageBuffer(ImageBuffer* imgBuf)
{
	_imageBuffer = imgBuf;
	return this;
}

void GLMeshBuilder::build()
{
	if(_vertexBuffer)
	{
		_vertexBufferID = GLCreateBuffer(_vertexBufferLen, (GLvoid*)_vertexBuffer);
	}

	if (_normalBuffer)
	{
		_normalBufferID = GLCreateBuffer(_normalBufferLen, (GLvoid*)_normalBuffer);
	}

	if (_uvBuffer)
	{
		_uvBufferID = GLCreateBuffer(_uvBufferLen, (GLvoid*)_uvBuffer);
	}

	if (_colorBuffer)
	{
		_colorBufferID = GLCreateBuffer(_colorBufferLen, (GLvoid*)_colorBuffer);
	}

	if (_indicesBuffer)
	{
		_indicesBufferID = GLCreateBuffer(_indicesBufferLen, (GLvoid*)_indicesBuffer);
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

GLuint GLMeshBuilder::GetVertexBufferID() 
{
	return _vertexBufferID;
}

GLuint GLMeshBuilder::GetNormalBufferID()
{
	return _normalBufferID;
}

GLuint GLMeshBuilder::GetUVBufferID()
{
	return _uvBufferID;
}

GLuint GLMeshBuilder::GetColorBufferID()
{
	return _colorBufferID;
}

GLuint GLMeshBuilder::GetIndicesBufferID()
{
	return _indicesBufferID;
}

GLuint GLMeshBuilder::GetBaseTexID()
{
	return _baseTexID;
}

GLuint GLMeshBuilder::GetVertexBufferSize()
{
	return _vertexBufferLen;
}

GLuint GLMeshBuilder::GetIndicesBufferSize()
{
	return _indicesBufferLen;
}

GLMeshBuilder::~GLMeshBuilder()
{
	if (_vertexBuffer)
	{
		GLDeleteBuffer(_vertexBufferID);
		_vertexBufferID = 0;
	}

	if (_normalBuffer)
	{
		GLDeleteBuffer(_normalBufferID);
		_normalBufferID = 0;
	}

	if (_uvBuffer)
	{
		GLDeleteBuffer(_uvBufferID);
		_uvBufferID = 0;
	}

	if (_colorBuffer)
	{
		GLDeleteBuffer(_colorBufferID);
		_colorBufferID = 0;
	}

	if (_imageBuffer)
	{
		GLDeleteTexture(_baseTexID);
		_baseTexID = 0;
	}
}