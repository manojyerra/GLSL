#include "ParticleReader.h"
#include "FileReader.h"
#include "ImageBuffer.h"
#include "GameLoop.h"
#include "Platform.h"

ParticleReader::ParticleReader(std::string folderPath)
{
	Platform::debugPrint("\nBegin: Loading particle data.");

	long startTime = Platform::GetTimeInMillis();

	_vertexFileReader = NULL;
	_colorBuf = NULL;
	_colorBufLen = 0;

	_lowPolyVertexBuf = NULL;
	_lowPolyVertexBufLen = 0;
	_lowPolyColorBuf = NULL;
	_lowPolyColorBufLen = 0;

	_retriveDataType = DATA_AS_IT_IS;

	if (FileReader::IsFileExists(folderPath + "/vertex.buf"))
	{
		_vertexFileReader = new FileReader(folderPath + "/vertex.buf", "rb");
	}

	// Generating color buffer.
	unsigned int fileLen = _vertexFileReader->GetLength();
	unsigned int vertexCount = fileLen / 12;

	_colorBufLen = vertexCount * 3;
	_colorBuf = (unsigned char*)malloc(_colorBufLen);
	memset(_colorBuf, 0, _colorBufLen);

	for (unsigned int i = 0; i < vertexCount; i++)
	{
		int ii = i * 3;

		_colorBuf[ii + 0] = 200;
		//_colorBuf[ii + 1] = 0;
		//_colorBuf[ii + 2] = 0;
	}

	// Generating low poly data
	GenerateLowPolyData(_vertexFileReader->GetData(), _vertexFileReader->GetLength());

	/*
	unsigned int vertexBufLen = 36;

	float* vertexBuf = (float*)malloc(vertexBufLen);

	float y = 0.0f;

	vertexBuf[0] = 0.0f;
	vertexBuf[1] = y;
	vertexBuf[2] = 0.0f;

	vertexBuf[3] = -5.0f;
	vertexBuf[4] = y;
	vertexBuf[5] = 0.0f;

	vertexBuf[6] = 5.0f;
	vertexBuf[7] = y;
	vertexBuf[8] = 0.0f;

	//_vertexBufferID = GLCreateBuffer(vertexBufLen, vertexBuf);

	unsigned int colorBufLen = 12;
	unsigned char* colorBuf = (unsigned char*)malloc(colorBufLen);

	colorBuf[0] = 255;
	colorBuf[1] = 0;
	colorBuf[2] = 0;

	colorBuf[3] = 0;
	colorBuf[4] = 255;
	colorBuf[5] = 0;

	colorBuf[6] = 0;
	colorBuf[7] = 0;
	colorBuf[8] = 255;

	//_colorBufferID = GLCreateBuffer(colorBufLen, colorBuf);

	_vertexCount = vertexBufLen / 12;
	//free(vertexBuf);

	_lowPolyVertexBuf = vertexBuf;
	_lowPolyVertexBufLen = vertexBufLen;
	_lowPolyColorBuf = colorBuf;
	_lowPolyColorBufLen = colorBufLen;
	*/

	Platform::debugPrint("\nEnd: Loading particle data, Load time : %ld", (Platform::GetTimeInMillis()-startTime));
}

void ParticleReader::GenerateLowPolyData(const char* fileData, unsigned int length)
{
	const unsigned int BYTES_PER_VERTEX = 12;
	const unsigned int skipNumVertex = 50;

	//Generating low poly vertex data
	unsigned int bpv = BYTES_PER_VERTEX;
	unsigned int skipBytes = skipNumVertex * bpv;
	unsigned int bufLen = length / (skipNumVertex - 1);

	_lowPolyVertexBuf = (unsigned char*)malloc(bufLen);

	unsigned int lowPolyVertexCount = 0;

	for (unsigned int i = 0, j = 0; i < length - skipBytes; i += skipBytes, j += bpv)
	{
		memcpy(&_lowPolyVertexBuf[j], &fileData[i], bpv);
		lowPolyVertexCount++;
	}

	_lowPolyVertexBufLen = lowPolyVertexCount * bpv;


	//Generating low poly color data
	_lowPolyColorBufLen = lowPolyVertexCount * 3;
	_lowPolyColorBuf = (unsigned char*)malloc(_lowPolyColorBufLen);
	memset(_lowPolyColorBuf, 0, _lowPolyColorBufLen);

	for (unsigned int i = 0; i < lowPolyVertexCount; i++)
	{
		int ii = i * 3;

		_lowPolyColorBuf[ii + 0] = 200;
		//_lowPolyColorBuf[ii + 1] = 0;
		//_lowPolyColorBuf[ii + 2] = 0;
	}
}

void ParticleReader::SetRetriveDataType(int retriveType)
{
	_retriveDataType = retriveType;
}

const char* ParticleReader::GetVertexBuffer()
{
	if(_retriveDataType == DATA_AS_IT_IS)
	{
		return (const char*)_vertexFileReader->GetData();
	}
	else if(_retriveDataType == DATA_AS_LOW_POLY)
	{
		return (const char*)_lowPolyVertexBuf;
	}

	return NULL;
}

const char* ParticleReader::GetColorBuffer()
{
	if (_retriveDataType == DATA_AS_IT_IS)
	{
		return (const char*)_colorBuf;
	}
	else if (_retriveDataType == DATA_AS_LOW_POLY)
	{
		return (const char*)_lowPolyColorBuf;
	}

	return NULL;
}

unsigned int ParticleReader::GetVertexBufferSize()
{
	if (_retriveDataType == DATA_AS_IT_IS)
	{
		return _vertexFileReader ? _vertexFileReader->GetLength() : 0;
	}
	else if (_retriveDataType == DATA_AS_LOW_POLY)
	{
		return _lowPolyVertexBufLen;
	}

	return 0;
}

unsigned int ParticleReader::GetColorBufferSize()
{
	if (_retriveDataType == DATA_AS_IT_IS)
	{
		return _colorBufLen;
	}
	else if (_retriveDataType == DATA_AS_LOW_POLY)
	{
		return _lowPolyColorBufLen;
	}

	return 0;
}

ParticleReader::~ParticleReader()
{
	if (_vertexFileReader)
	{
		delete _vertexFileReader;
		_vertexFileReader = NULL;
	}

	if(_colorBuf)
	{
		free(_colorBuf);
		_colorBuf = NULL;
	}

	if (_lowPolyVertexBuf)
	{
		free(_lowPolyVertexBuf);
		_lowPolyVertexBuf = NULL;
	}

	if (_lowPolyColorBuf)
	{
		free(_lowPolyColorBuf);
		_lowPolyColorBuf = NULL;
	}
}
