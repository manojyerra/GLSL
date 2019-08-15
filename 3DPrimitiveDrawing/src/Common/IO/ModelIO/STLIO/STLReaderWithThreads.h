#ifndef STLReaderWithThreads_H
#define STLReaderWithThreads_H

#include "FloatArray.h"
#include "VoidPtrArray.h"
#include "BaseModelIO.h"
#include "ScanLineUtils.h"
#include "Platform.h"

class STLScanLines
{
private:
	FloatArray* _vertexFloatArr;
	FloatArray* _normalFloatArr;
	VoidPtrArray* _linesVec;
	AABB _bBox;

	unsigned int _startI;
	unsigned int _endI;

public:
	STLScanLines(VoidPtrArray* linesVec, unsigned int startI, unsigned int endI)
	{
		_linesVec = linesVec;

		_startI = startI;
		_endI = endI;

		_vertexFloatArr = nullptr;
		_normalFloatArr = nullptr;
	}

	FloatArray* GetVertexArrary()
	{
		return _vertexFloatArr;
	}

	FloatArray* GetNormalArrary()
	{
		return _normalFloatArr;
	}

	void run()
	{
		float vx, vy, vz;
		float nx, ny, nz;
		bool isFirstVertex = true;

		unsigned int initSize = 1024 * 1024;
		_vertexFloatArr = new FloatArray(initSize * 4);
		_normalFloatArr = new FloatArray(initSize * 4);

		for(int i=_startI; i<=_endI; i++ )
		{
			char* line = (char*)_linesVec->get(i);

			if (line[0] == 'f')
			{
				ScanLineUtils::scan_vertex(line, 12, &nx, &ny, &nz);
			}
			else if (line[0] == 'v')
			{
				ScanLineUtils::scan_vertex(line, 6, &vx, &vy, &vz);

				_vertexFloatArr->push_back_3(vx, vy, vz);
				_normalFloatArr->push_back_3(nx, ny, nz);

				if (isFirstVertex)
				{
					_bBox.minX = _bBox.maxX = vx;
					_bBox.minY = _bBox.maxY = vy;
					_bBox.minZ = _bBox.maxZ = vz;

					isFirstVertex = false;
				}
				else
				{
					if (vx < _bBox.minX)		_bBox.minX = vx;
					else if (vx > _bBox.maxX)	_bBox.maxX = vx;

					if (vy < _bBox.minY)		_bBox.minY = vy;
					else if (vy >_bBox.maxY)	_bBox.maxY = vy;

					if (vz < _bBox.minZ)		_bBox.minZ = vz;
					else if (vz > _bBox.maxZ)	_bBox.maxZ = vz;
				}
			}
		}
	}

	~STLScanLines()
	{
		if (_vertexFloatArr)
		{
			delete _vertexFloatArr;
			_vertexFloatArr = nullptr;
		}

		if(_normalFloatArr)
		{
			delete _normalFloatArr;
			_normalFloatArr = nullptr;
		}
	}
};

class STLReaderWithThreads : public BaseModelIO
{
private:
	std::string _filePath;

	VoidPtrArray* _linesVec;
	FloatArray* _vertexFloatArr;
	FloatArray* _normalFloatArr;

	void ReadSTLFile(std::string filePath);

public:
	STLReaderWithThreads(std::string filePath);

	const char* GetVertexBuffer() override;
	const char* GetNormalBuffer() override;

	unsigned int GetVertexBufferSize() override;
	unsigned int GetNormalBufferSize() override;

	void WriteBinaryToFile();
	void WriteBinaryToFile(std::string folderPath);

	~STLReaderWithThreads();
};

#endif


//class Timer
//{
//private:
//	long startTime;
//
//public:
//	Timer()
//	{
//		startTime = Platform::GetTimeInMillis();
//	}
//
//	Timer(const char* str)
//	{
//		startTime = Platform::GetTimeInMillis();
//		Platform::debugPrint("\n%s", str);
//	}
//
//	void Stop(const char* str)
//	{
//		long timeTaken = Platform::GetTimeInMillis() - startTime;
//		printf("\n%s : %ld", str, timeTaken);
//	}
//};
