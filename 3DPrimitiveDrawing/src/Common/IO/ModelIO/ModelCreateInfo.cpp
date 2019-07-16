#include "ModelCreateInfo.h"

ModelCreateInfo::ModelCreateInfo()
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

void ModelCreateInfo::SetVertexBuffer(const char* buf, unsigned int len)
{
	_vertexBuf = buf;
	_vertexBufLen = len;
}

void ModelCreateInfo::SetUVBuffer(const char* buf, unsigned int len)
{
	_uvBuf = buf;
	_uvBufLen = len;
}

void ModelCreateInfo::SetNormalBuffer(const char* buf, unsigned int len)
{
	_normalBuf = buf;
	_normalBufLen = len;
}

void ModelCreateInfo::SetColorBuffer(const char* buf, unsigned int len)
{
	_colorBuf = buf;
	_colorBufLen = len;
}

void ModelCreateInfo::SetIndicesBuffer(const char* buf, unsigned int len)
{
	_indicesBuf = buf;
	_indicesBufLen = len;
}

void ModelCreateInfo::SetImageBufferInfo(ImageBuffer* imageBuf)
{
	_imageBuf = imageBuf;
}

const char* ModelCreateInfo::GetVertexBuffer() { return _vertexBuf; }
const char* ModelCreateInfo::GetUVBuffer() { return _uvBuf; }
const char* ModelCreateInfo::GetNormalBuffer() { return _normalBuf; }
const char* ModelCreateInfo::GetColorBuffer() { return _colorBuf; }
const char* ModelCreateInfo::GetIndicesBuffer() { return _indicesBuf; }
ImageBuffer* ModelCreateInfo::GetImageBuffer() { return _imageBuf; }

unsigned int ModelCreateInfo::GetVertexBufferSize() { return _vertexBufLen; }
unsigned int ModelCreateInfo::GetUVBufferSize() { return _uvBufLen; }
unsigned int ModelCreateInfo::GetNormalBufferSize() { return _normalBufLen; }
unsigned int ModelCreateInfo::GetColorBufferSize() { return _colorBufLen; }
unsigned int ModelCreateInfo::GetIndicesBufferSize() { return _indicesBufLen;  }

ModelCreateInfo::~ModelCreateInfo()
{
}
