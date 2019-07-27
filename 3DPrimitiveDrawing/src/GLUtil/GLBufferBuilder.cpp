#include "GLBufferBuilder.h"
#include "GLMemory.h"

GLBufferBuilder::GLBufferBuilder()
{
	_vertexBufferID = 0;
	_normalBufferID = 0;
	_uvBufferID = 0;
	_colorBufferID = 0;
	_indicesBufferID = 0;
	_baseTexID = 0;

	_vertexBuffer = nullptr;
	_normalBuffer = nullptr;
	_uvBuffer = nullptr;
	_colorBuffer = nullptr;
	_indicesBuffer = nullptr;
	_imageBuffer = nullptr;

	_vertexBufferLen = 0;
	_normalBufferLen = 0;
	_uvBufferLen = 0;
	_colorBufferLen = 0;
	_indicesBufferLen = 0;
}

GLBufferBuilder* GLBufferBuilder::SetVertexBuffer(const GLvoid* buffer, GLsizeiptr len)
{
	_vertexBuffer = buffer;
	_vertexBufferLen = len;
	return this;
}

GLBufferBuilder* GLBufferBuilder::SetNormalBuffer(const GLvoid* buffer, GLsizeiptr len)
{
	_normalBuffer = buffer;
	_normalBufferLen = len;
	return this;
}

GLBufferBuilder* GLBufferBuilder::SetUVBuffer(const GLvoid* buffer, GLsizeiptr len)
{
	_uvBuffer = buffer;
	_uvBufferLen = len;
	return this;
}

GLBufferBuilder* GLBufferBuilder::SetColorBuffer(const GLvoid* buffer, GLsizeiptr len)
{
	_colorBuffer = buffer;
	_colorBufferLen = len;
	return this;
}

GLBufferBuilder* GLBufferBuilder::SetIndicesBuffer(const GLvoid* buffer, GLsizeiptr len)
{
	_indicesBuffer = buffer;
	_indicesBufferLen = len;
	return this;
}

GLBufferBuilder* GLBufferBuilder::SetImageBuffer(ImageBuffer* imgBuf)
{
	_imageBuffer = imgBuf;
	return this;
}

void GLBufferBuilder::build()
{
	//unsigned int vao;
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	if (_vertexBuffer)
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

GLuint GLBufferBuilder::GetVertexBufferID()
{
	return _vertexBufferID;
}

GLuint GLBufferBuilder::GetNormalBufferID()
{
	return _normalBufferID;
}

GLuint GLBufferBuilder::GetUVBufferID()
{
	return _uvBufferID;
}

GLuint GLBufferBuilder::GetColorBufferID()
{
	return _colorBufferID;
}

GLuint GLBufferBuilder::GetIndicesBufferID()
{
	return _indicesBufferID;
}

GLuint GLBufferBuilder::GetBaseTexID()
{
	return _baseTexID;
}

GLsizeiptr GLBufferBuilder::GetVertexBufferSize()
{
	return _vertexBufferLen;
}

GLsizeiptr GLBufferBuilder::GetIndicesBufferSize()
{
	return _indicesBufferLen;
}

GLBufferBuilder::~GLBufferBuilder()
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