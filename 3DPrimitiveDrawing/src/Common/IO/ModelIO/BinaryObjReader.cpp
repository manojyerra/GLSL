#include "BinaryObjReader.h"
#include "FileReader.h"
#include "ImageBuffer.h"

BinaryObjReader::BinaryObjReader(string folderPath)
{
	_vertexFileReader = NULL;
	_uvFileReader = NULL;
	_normalFileReader = NULL;

	_vertexCount = 0;
	_imageBuffer = NULL;

	ReadObjFile(folderPath);
	LoadTextures(folderPath);
}

void BinaryObjReader::ReadObjFile(string folderPath)
{
	unsigned int startTime = GetTickCount();

	if (FileReader::IsFileExists(folderPath + "/vertex.buf"))
	{
		_vertexFileReader = new FileReader(folderPath + "/vertex.buf", "rb");
		_vertexCount = _vertexFileReader->GetLength() / 12;
	}

	if (FileReader::IsFileExists(folderPath + "/normal.buf"))
	{
		_uvFileReader = new FileReader(folderPath + "/normal.buf", "rb");		
	}

	if (FileReader::IsFileExists(folderPath + "/uv.buf"))
	{
		_normalFileReader = new FileReader(folderPath + "/uv.buf", "rb");
	}
}

const float* BinaryObjReader::GetVertexArr()
{
	return (const float*)_vertexFileReader->GetData();
}

const float* BinaryObjReader::GetUVArr()
{
	return (const float*)_uvFileReader->GetData();
}

const float* BinaryObjReader::GetNormalArr()
{
	return (const float*)_normalFileReader->GetData();
}

unsigned int BinaryObjReader::GetVertexCount()
{
	return _vertexCount;
}

ImageBuffer* BinaryObjReader::GetImageBuffer()
{
	return _imageBuffer;
}

void BinaryObjReader::LoadTextures(string folderPath)
{
	string filePath = folderPath + "/texture.png";

	if (FileReader::IsFileExists(filePath))
	{
		_imageBuffer = new ImageBuffer(filePath);
	}
}

BinaryObjReader::~BinaryObjReader()
{
	if (_imageBuffer)
	{
		delete _imageBuffer;
		_imageBuffer = NULL;
	}

	if (_vertexFileReader)
	{
		delete _vertexFileReader;
		_vertexFileReader = NULL;
	}

	if (_uvFileReader)
	{
		delete _uvFileReader;
		_uvFileReader = NULL;
	}

	if (_normalFileReader)
	{
		delete _normalFileReader;
		_normalFileReader = NULL;
	}
}
