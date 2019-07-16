#ifndef ObjReader_H
#define ObjReader_H

#include "FloatArray.h"
#include "ULongArray.h"
#include "VoidPtrArray.h"
#include "ModelCreateInfo.h"

class ObjReader : public ModelCreateInfo
{
private:
	string _folderPath;

	FloatArray* _vertexFloatArr;
	FloatArray* _uvFloatArr;
	FloatArray* _normalFloatArr;
	ImageBuffer* _imageBuffer;

	void ReadObjFile(string folderPath);
	void LoadTextures(string folderPath);

public:
	ObjReader(string folderPath);

	const char* GetVertexBuffer();
	const char* GetUVBuffer();
	const char* GetNormalBuffer();
	ImageBuffer* GetImageBuffer();

	unsigned int GetVertexBufferSize();
	unsigned int GetUVBufferSize();
	unsigned int GetNoralBufferSize();

	void WriteBinaryToFile();
	void WriteBinaryToFile(string folderPath);
	~ObjReader();
};

#endif