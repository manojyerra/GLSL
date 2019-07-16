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

	const char* GetVertexBuffer() override;
	const char* GetUVBuffer() override;
	const char* GetNormalBuffer() override;
	ImageBuffer* GetImageBuffer() override;

	unsigned int GetVertexBufferSize() override;
	unsigned int GetUVBufferSize() override;
	unsigned int GetNormalBufferSize() override;

	void WriteBinaryToFile();
	void WriteBinaryToFile(string folderPath);
	~ObjReader();
};

#endif