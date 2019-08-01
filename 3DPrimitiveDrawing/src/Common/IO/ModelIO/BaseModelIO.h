#ifndef BaseModelIO_H
#define BaseModelIO_H

#include "ImageBuffer.h"
#include "AABB.h"

class BaseModelIO
{
private:
	const char* _vertexBuf;
	const char* _uvBuf;
	const char* _normalBuf;
	const char* _colorBuf;
	const char* _indicesBuf;
	ImageBuffer* _imageBuf;

	unsigned int _vertexBufLen;
	unsigned int _uvBufLen;
	unsigned int _normalBufLen;
	unsigned int _colorBufLen;
	unsigned int _indicesBufLen;

protected:
	AABB bBox;

public:
	enum
	{
		OBJ_MODEL,
		BINARY_OBJ_MODEL,
		STL_MODEL,
		PARTICLE_DATA
	};

	BaseModelIO();
	~BaseModelIO();
	
	virtual void SetVertexBuffer(const char* buf, unsigned int len);
	virtual void SetUVBuffer(const char* buf, unsigned int len);
	virtual void SetNormalBuffer(const char* buf, unsigned int len);
	virtual void SetColorBuffer(const char* buf, unsigned int len);
	virtual void SetIndicesBuffer(const char* buf, unsigned int len);
	virtual void SetImageBuffer(ImageBuffer* imageBuf);

	virtual const char* GetVertexBuffer();
	virtual const char* GetUVBuffer();
	virtual const char* GetNormalBuffer();
	virtual const char* GetColorBuffer();
	virtual const char* GetIndicesBuffer();
	virtual ImageBuffer* GetImageBuffer();

	virtual unsigned int GetVertexBufferSize();
	virtual unsigned int GetUVBufferSize();
	virtual unsigned int GetNormalBufferSize();
	virtual unsigned int GetColorBufferSize();
	virtual unsigned int GetIndicesBufferSize();

	virtual AABB GetAABB();
};

#endif