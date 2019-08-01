#ifndef BinaryObjReader_H
#define BinaryObjReader_H

#include "FileReader.h"
#include "BaseModelIO.h"

class BinaryObjReader : public BaseModelIO
{
private:
	FileReader* _vertexFileReader;
	FileReader* _uvFileReader;
	FileReader* _normalFileReader;

	ImageBuffer* _imageBuffer;

	void ReadBinaryObjFile(std::string folderPath);
	void LoadTextures(std::string folderPath);
	   
public:
	BinaryObjReader(std::string folderPath);
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