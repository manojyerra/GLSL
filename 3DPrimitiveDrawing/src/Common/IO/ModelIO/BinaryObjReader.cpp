#include "BinaryObjReader.h"
#include "FileReader.h"
#include "ImageBuffer.h"

BinaryObjReader::BinaryObjReader(string folderPath)
{
	_vertexFileReader = NULL;
	_uvFileReader = NULL;
	_normalFileReader = NULL;

	_imageBuffer = NULL;

	ReadObjFile(folderPath);
	LoadTextures(folderPath);
}

void BinaryObjReader::ReadObjFile(string folderPath)
{
	if (FileReader::IsFileExists(folderPath + "/vertex.buf"))
	{
		_vertexFileReader = new FileReader(folderPath + "/vertex.buf", "rb");
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


const char* BinaryObjReader::GetVertexBuffer()
{
	return (const char*)_vertexFileReader->GetData();
}

const char* BinaryObjReader::GetUVBuffer()
{
	return (const char*)_uvFileReader->GetData();
}

const char* BinaryObjReader::GetNormalBuffer()
{
	return (const char*)_normalFileReader->GetData();
}

ImageBuffer* BinaryObjReader::GetImageBuffer()
{
	return _imageBuffer;
}

unsigned int BinaryObjReader::GetVertexBufferSize()
{
	return _vertexFileReader ? _vertexFileReader->GetLength() : 0;
}

unsigned int BinaryObjReader::GetUVBufferSize()
{
	return _uvFileReader ? _uvFileReader->GetLength() : 0;
}

unsigned int BinaryObjReader::GetNoralBufferSize()
{
	return _normalFileReader ? _normalFileReader->GetLength() : 0;
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
