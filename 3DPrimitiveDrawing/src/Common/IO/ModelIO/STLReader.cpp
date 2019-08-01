#include "STLReader.h"
#include "FileReader.h"
#include "ScanLineUtils.h"
#include "Platform.h"
#include "ObjReader.h"
#include "StringUtils.h"

STLReader::STLReader(std::string filePath)
{
	_filePath = filePath;

	_vertexFloatArr = NULL;
	_normalFloatArr = NULL;

	long startTime = Platform::GetTimeInMillis();
	Platform::debugPrint("\nBegin: Loading STL file %s, ", _filePath.c_str());
	ReadSTLFile(_filePath);
	Platform::debugPrint("\nEnd: Loading STL file %s, Load Time : %ld milliseconds\n", _filePath.c_str(), Platform::GetTimeInMillis() - startTime);
}

void STLReader::ReadSTLFile(std::string filePath)
{
	FileReader fileReader(filePath, "rb");

	float vx, vy, vz;
	float nx, ny, nz;
	bool isFirstVertex = true;

	char* line = NULL;

	unsigned int initSize = 1024 * 1024;
	_vertexFloatArr = new FloatArray(initSize * 4);
	_normalFloatArr = new FloatArray(initSize * 4);

	while ((line = fileReader.ReadLine()) != NULL)
	{
		int i = 0;

		while (line[i] == ' ')
		{
			i++;
		}

		if (line[i] == 'f')
		{
			ScanLineUtils::scan_vertex(line, i+13, &nx, &ny, &nz);
		}
		else if(line[i] == 'v')
		{
			ScanLineUtils::scan_vertex(line, i+7, &vx, &vy, &vz);

			_vertexFloatArr->push_back_3(vx, vy, vz);
			_normalFloatArr->push_back_3(nx, ny, nz);

			if (isFirstVertex)
			{
				bBox.minX = bBox.maxX = vx;
				bBox.minY = bBox.maxY = vy;
				bBox.minZ = bBox.maxZ = vz;

				isFirstVertex = false;
			}
			else
			{
				if (vx < bBox.minX)			bBox.minX = vx;
				else if (vx > bBox.maxX)	bBox.maxX = vx;

				if (vy < bBox.minY)			bBox.minY = vy;
				else if (vy > bBox.maxY)	bBox.maxY = vy;

				if (vz < bBox.minZ)			bBox.minZ = vz;
				else if (vz > bBox.maxZ)	bBox.maxZ = vz;
			}
		}
	}
}

void STLReader::WriteBinaryToFile()
{
	std::string folderPath = StringUtils::getFolderNameFromPath(_filePath);
	WriteBinaryToFile(folderPath);
}

void STLReader::WriteBinaryToFile(std::string folderPath)
{
	if (_vertexFloatArr)
	{
		std::string bufFilePath = folderPath + "/vertex.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(_vertexFloatArr->getArray(), sizeof(float), _vertexFloatArr->size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}

	if (_normalFloatArr)
	{
		std::string bufFilePath = folderPath + "/normal.buf";
		FILE* bufFile = fopen(bufFilePath.c_str(), "wb");
		fwrite(_normalFloatArr->getArray(), sizeof(float), _normalFloatArr->size(), bufFile);
		fflush(bufFile);
		fclose(bufFile);
	}
}

const char* STLReader::GetVertexBuffer()
{
	return (_vertexFloatArr ? (const char*)_vertexFloatArr->getArray() : NULL);
}

const char* STLReader::GetNormalBuffer()
{
	return (_normalFloatArr ? (const char*)_normalFloatArr->getArray() : NULL);
}

//TODO : Think of having sizeInBytes method in FloatArray class instead of multiplying it with sizeof(float)
unsigned int STLReader::GetVertexBufferSize()
{
	return _vertexFloatArr ? _vertexFloatArr->size() * sizeof(float) : 0;
}

unsigned int STLReader::GetNormalBufferSize()
{
	return _normalFloatArr ? _normalFloatArr->size() * sizeof(float) : 0;
}

STLReader::~STLReader()
{
	if (_vertexFloatArr)
	{
		delete _vertexFloatArr;
		_vertexFloatArr = NULL;
	}

	if (_normalFloatArr)
	{
		delete _normalFloatArr;
		_normalFloatArr = NULL;
	}
}
