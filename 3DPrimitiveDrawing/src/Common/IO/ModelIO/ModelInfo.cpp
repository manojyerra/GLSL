#include "ModelInfo.h"

ModelInfo::ModelInfo()
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

void ModelInfo::SetVertexBuffer(const char* buf, unsigned int len)
{
	_vertexBuf = buf;
	_vertexBufLen = len;
}

void ModelInfo::SetUVBuffer(const char* buf, unsigned int len)
{
	_uvBuf = buf;
	_uvBufLen = len;
}

void ModelInfo::SetNormalBuffer(const char* buf, unsigned int len)
{
	_normalBuf = buf;
	_normalBufLen = len;
}

void ModelInfo::SetColorBuffer(const char* buf, unsigned int len)
{
	_colorBuf = buf;
	_colorBufLen = len;
}

void ModelInfo::SetIndicesBuffer(const char* buf, unsigned int len)
{
	_indicesBuf = buf;
	_indicesBufLen = len;
}

void ModelInfo::SetImageBuffer(ImageBuffer* imageBuf)
{
	_imageBuf = imageBuf;
}

const char* ModelInfo::GetVertexBuffer() { return _vertexBuf; }
const char* ModelInfo::GetUVBuffer() { return _uvBuf; }
const char* ModelInfo::GetNormalBuffer() { return _normalBuf; }
const char* ModelInfo::GetColorBuffer() { return _colorBuf; }
const char* ModelInfo::GetIndicesBuffer() { return _indicesBuf; }
ImageBuffer* ModelInfo::GetImageBuffer() { return _imageBuf; }

unsigned int ModelInfo::GetVertexBufferSize() { return _vertexBufLen; }
unsigned int ModelInfo::GetUVBufferSize() { return _uvBufLen; }
unsigned int ModelInfo::GetNormalBufferSize() { return _normalBufLen; }
unsigned int ModelInfo::GetColorBufferSize() { return _colorBufLen; }
unsigned int ModelInfo::GetIndicesBufferSize() { return _indicesBufLen;  }

AABB ModelInfo::GetAABB()
{
	return bBox;
}

ModelInfo::~ModelInfo()
{
}
