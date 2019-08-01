#ifndef ObjReader_H
#define ObjReader_H

#include "FloatArray.h"
#include "BaseModelIO.h"

class ObjReader : public BaseModelIO
{
private:
	std::string _filePath;

	FloatArray* _vertexFloatArr;
	FloatArray* _uvFloatArr;
	FloatArray* _normalFloatArr;
	ImageBuffer* _imageBuffer;

	void ReadObjFile(std::string filePath);
	void LoadTextures(std::string filePath);

public:
	ObjReader(std::string filePath);

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