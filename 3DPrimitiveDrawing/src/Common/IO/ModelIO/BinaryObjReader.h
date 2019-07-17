#ifndef BinaryObjReader_H
#define BinaryObjReader_H

#include "FileReader.h"
#include "ModelInfo.h"

class BinaryObjReader : public ModelInfo
{
private:
	FileReader* _vertexFileReader;
	FileReader* _uvFileReader;
	FileReader* _normalFileReader;

	ImageBuffer* _imageBuffer;

	void ReadObjFile(string folderPath);
	void LoadTextures(string folderPath);
	   
public:
	BinaryObjReader(string folderPath);
	~BinaryObjReader();

	const char* GetVertexBuffer() override;
	const char* GetUVBuffer() override;
	const char* GetNormalBuffer() override;
	ImageBuffer* GetImageBuffer() override;

	unsigned int GetVertexBufferSize() override;
	unsigned int GetUVBufferSize() override;
	unsigned int GetNormalBufferSize() override;
};

#endif