#include "BaseModelIO.h"

BaseModelIO::BaseModelIO()
{
	_vertexBuf = NULL;
	_uvBuf = NULL;
	_normalBuf = NULL;
	_colorBuf = NULL;
	_indicesBuf = NULL;
	_imageBuf = NULL;

	_vertexBufLen = 0;
	_uvBufLen = 0;
	_normalBufLen = 0;
	_colorBufLen = 0;
	_indicesBufLen = 0;
}

void BaseModelIO::SetVertexBuffer(const char* buf, unsigned int len)
{
	_vertexBuf = buf;
	_vertexBufLen = len;
}

void BaseModelIO::SetUVBuffer(const char* buf, unsigned int len)
{
	_uvBuf = buf;
	_uvBufLen = len;
}

void BaseModelIO::SetNormalBuffer(const char* buf, unsigned int len)
{
	_normalBuf = buf;
	_normalBufLen = len;
}

void BaseModelIO::SetColorBuffer(const char* buf, unsigned int len)
{
	_colorBuf = buf;
	_colorBufLen = len;
}

void BaseModelIO::SetIndicesBuffer(const char* buf, unsigned int len)
{
	_indicesBuf = buf;
	_indicesBufLen = len;
}

void BaseModelIO::SetImageBuffer(ImageBuffer* imageBuf)
{
	_imageBuf = imageBuf;
}

void BaseModelIO::SetVertexBufferInfo(BufferInfo* buf) { SetVertexBuffer(buf->buffer, buf->size); }
void BaseModelIO::SetUVBufferInfo(BufferInfo* buf) { SetUVBuffer(buf->buffer, buf->size); }
void BaseModelIO::SetNormalBufferInfo(BufferInfo* buf) { SetNormalBuffer(buf->buffer, buf->size); }
void BaseModelIO::SetColorBufferInfo(BufferInfo* buf) { SetColorBuffer(buf->buffer, buf->size); }
void BaseModelIO::SetIndicesBufferInfo(BufferInfo* buf) { SetIndicesBuffer(buf->buffer, buf->size); }

const char* BaseModelIO::GetVertexBuffer() { return _vertexBuf; }
const char* BaseModelIO::GetUVBuffer() { return _uvBuf; }
const char* BaseModelIO::GetNormalBuffer() { return _normalBuf; }
const char* BaseModelIO::GetColorBuffer() { return _colorBuf; }
const char* BaseModelIO::GetIndicesBuffer() { return _indicesBuf; }
ImageBuffer* BaseModelIO::GetImageBuffer() { return _imageBuf; }

unsigned int BaseModelIO::GetVertexBufferSize() { return _vertexBufLen; }
unsigned int BaseModelIO::GetUVBufferSize() { return _uvBufLen; }
unsigned int BaseModelIO::GetNormalBufferSize() { return _normalBufLen; }
unsigned int BaseModelIO::GetColorBufferSize() { return _colorBufLen; }
unsigned int BaseModelIO::GetIndicesBufferSize() { return _indicesBufLen;  }

AABB BaseModelIO::GetAABB()
{
	return bBox;
}

BaseModelIO::~BaseModelIO()
{
}
