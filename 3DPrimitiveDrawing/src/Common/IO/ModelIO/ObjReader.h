#ifndef ObjReader_H
#define ObjReader_H

#include "FloatArray.h"
#include "ModelInfo.h"

class ObjReader : public ModelInfo
{
private:
	std::string _folderPath;

	FloatArray* _vertexFloatArr;
	FloatArray* _uvFloatArr;
	FloatArray* _normalFloatArr;
	ImageBuffer* _imageBuffer;

	void ReadObjFile(std::string folderPath);
	void LoadTextures(std::string folderPath);

public:
	ObjReader(std::string folderPath);

	const char* GetVertexBuffer() override;
	const char* GetUVBuffer() override;
	const char* GetNormalBuffer() override;
	ImageBuffer* GetImageBuffer() override;

	unsigned int GetVertexBufferSize() override;
	unsigned int GetUVBufferSize() override;
	unsigned int GetNormalBufferSize() override;

	void WriteBinaryToFile();
	void WriteBinaryToFile(std::string folderPath);
	~ObjReader();
};

#endif