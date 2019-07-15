#ifndef ObjReader_H
#define ObjReader_H

#include "ImageBuffer.h"
#include "FloatArray.h"
#include "ULongArray.h"
#include "VoidPtrArray.h"

#include <vector>
#include <string>
using namespace std;


class ObjReader
{
private:
	FloatArray* _vertexFloatArr;
	FloatArray* _uvFloatArr;
	FloatArray* _normalFloatArr;

	unsigned int _vertexCount;

	ImageBuffer* _imageBuffer;

	string _folderPath;

	bool _writeBinaryToFile;

	void ReadObjFile(string folderPath);
	void LoadTextures(string folderPath);

public:
	ObjReader(string folderPath);

	const char* GetVertexBuffer();
	const char* GetUVBuffer();
	const char* GetNormalBuffer();
	unsigned int GetVertexCount();
	ImageBuffer* GetImageBuffer();

	unsigned int GetVertexBufferSize();
	unsigned int GetUVBufferSize();
	unsigned int GetNoralBufferSize();

	void WriteBinaryToFile();
	void WriteBinaryToFile(string folderPath);
	~ObjReader();
};

#endif