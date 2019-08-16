#include "STLReaderWithThreads.h"
#include "FileReader.h"
#include "ScanLineUtils.h"
#include "Platform.h"
#include "ObjReader.h"
#include "StringUtils.h"
#include "VoidPtrArray.h"
#include <thread>

STLReaderWithThreads::STLReaderWithThreads(std::string filePath)
{
	_filePath = filePath;
	_linesVec = nullptr;
	_vertexFloatArr = nullptr;
	_normalFloatArr = nullptr;
	
	long startTime = Platform::GetTimeInMillis();
	Platform::debugPrint("\nBegin: Loading STL(With Threads) file %s, ", _filePath.c_str());
	ReadSTLFile(_filePath);
	Platform::debugPrint("\nEnd: Loading STL(With Threads) file %s, Load Time : %ld milliseconds\n", _filePath.c_str(), Platform::GetTimeInMillis() - startTime);
}

void STLReaderWithThreads::ReadSTLFile(std::string filePath)
{
	FileReader fileReader(filePath, "rb");

	_linesVec = new VoidPtrArray(1024 * 1024);

	char* line = NULL;

	while ((line = fileReader.ReadLine()) != NULL)
	{
		int i = -1;
		while (line[++i] == ' ');

		if (line[i] == 'f' || line[i] == 'v')
		{
			_linesVec->push_back(&line[i]);
		}
	}

	unsigned int numThreads = 4;
	unsigned int numLinesForTri = 4;
	unsigned int numTriangles = _linesVec->size() / numLinesForTri;
	unsigned int numTrisForThread = numTriangles / numThreads;

	unsigned int startI = 0;
	unsigned int endI = 0;

	std::vector<STLScanLines*> scanLinesVec;
	std::vector<std::thread*> threadsVec;

	for (int i = 0; i < numThreads; i++)
	{
		if (i == numThreads-1)
		{
			startI = i * numTrisForThread * numLinesForTri;
			endI = numTriangles * numLinesForTri;
		}
		else
		{
			startI = i * numTrisForThread * numLinesForTri;
			endI = (i + 1) * numTrisForThread * numLinesForTri;
		}

		//printf("\nstart = %d, end = %d", startI, (endI - 1));

		STLScanLines* scanLineObj = new STLScanLines(_linesVec, startI, (endI-1));
		scanLinesVec.push_back(scanLineObj);

		std::thread* threadObj = new std::thread(&STLScanLines::run, scanLineObj);
		threadsVec.push_back(threadObj);
	}
	
	for (int i = 0; i < threadsVec.size(); i++)
	{
		threadsVec[i]->join();
	}

	unsigned int vertexArrSize = 0;
	unsigned int normalArrSize = 0;

	for (int i = 0; i < scanLinesVec.size(); i++)
	{
		vertexArrSize += scanLinesVec[i]->GetVertexArrary()->size();
		normalArrSize += scanLinesVec[i]->GetNormalArrary()->size();
	}
	
	_vertexFloatArr = new FloatArray(vertexArrSize);
	_normalFloatArr = new FloatArray(normalArrSize);

	for (int i = 0; i < scanLinesVec.size(); i++)
	{
		_vertexFloatArr->Append(scanLinesVec[i]->GetVertexArrary());
		_normalFloatArr->Append(scanLinesVec[i]->GetNormalArrary());
	}

	for (int i = 0; i < scanLinesVec.size(); i++)
	{
		delete scanLinesVec[i];
		scanLinesVec[i] = nullptr;
	}

	for (int i = 0; i < threadsVec.size(); i++)
	{
		if (threadsVec[i])
		{
			delete threadsVec[i];
			threadsVec[i] = nullptr;
		}
	}
}

void STLReaderWithThreads::WriteBinaryToFile()
{
	std::string folderPath = StringUtils::getFolderNameFromPath(_filePath);
	WriteBinaryToFile(folderPath);
}

void STLReaderWithThreads::WriteBinaryToFile(std::string folderPath)
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

const char* STLReaderWithThreads::GetVertexBuffer()
{
	return (_vertexFloatArr ? (const char*)_vertexFloatArr->getArray() : NULL);
}

const char* STLReaderWithThreads::GetNormalBuffer()
{
	return (_normalFloatArr ? (const char*)_normalFloatArr->getArray() : NULL);
}

//TODO : Think of having sizeInBytes method in FloatArray class instead of multiplying it with sizeof(float)
unsigned int STLReaderWithThreads::GetVertexBufferSize()
{
	return _vertexFloatArr ? _vertexFloatArr->size() * sizeof(float) : 0;
}

unsigned int STLReaderWithThreads::GetNormalBufferSize()
{
	return _normalFloatArr ? _normalFloatArr->size() * sizeof(float) : 0;
}

STLReaderWithThreads::~STLReaderWithThreads()
{
	if (_vertexFloatArr)
	{
		delete _vertexFloatArr;
		_vertexFloatArr = nullptr;
	}

	if (_normalFloatArr)
	{
		delete _normalFloatArr;
		_normalFloatArr = nullptr;
	}

	if (_linesVec)
	{
		delete _linesVec;
		_linesVec = nullptr;
	}
}


/*
class STLReadThread
{
private:
	FloatArray* _vertexFloatArr;
	FloatArray* _normalFloatArr;
	VoidPtrArray* _data;
	unsigned int _dataIndex;
	bool _runLoop;

public:
	STLReadThread()
	{
		unsigned int initSize = 1024 * 1024;
		_vertexFloatArr = new FloatArray(initSize * 4);
		_normalFloatArr = new FloatArray(initSize * 4);
		_data = new VoidPtrArray(initSize);
		_dataIndex = 0;
		_runLoop = true;
	}

	void Stop()
	{
		_runLoop = false;
	}

	void AddData(char* line)
	{
		_data->push_back(line);
	}

	void Run()
	{
		while (_runLoop || _dataIndex < _data->size())
		{
			if (_dataIndex < _data->size())
			{
				ScanLine((char*)_data->get(_dataIndex));
				_dataIndex++;
			}
		}
	}

	void ScanLine(char* line)
	{
		int i = -1;
		while (line[++i] == ' ');

		float vx, vy, vz;
		float nx, ny, nz;

		if (line[i] == 'f')
		{
			ScanLineUtils::scan_vertex(line, i + 12, &nx, &ny, &nz);
		}
		else if (line[i] == 'v')
		{
			ScanLineUtils::scan_vertex(line, i + 6, &vx, &vy, &vz);

			_vertexFloatArr->push_back_3(vx, vy, vz);
			_normalFloatArr->push_back_3(nx, ny, nz);
		}
	}
};
*/