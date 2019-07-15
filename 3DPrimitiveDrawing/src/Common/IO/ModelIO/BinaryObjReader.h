#ifndef BinaryObjReader_H
#define BinaryObjReader_H

#include "FileReader.h"
#include "ImageBuffer.h"

class BinaryObjReader
{
private:
	FileReader* _vertexFileReader;
	FileReader* _uvFileReader;
	FileReader* _normalFileReader;

	unsigned int _vertexCount;

	ImageBuffer* _imageBuffer;

	void ReadObjFile(string folderPath);
	void LoadTextures(string folderPath);
	   
public:
	BinaryObjReader(string folderPath);
	~BinaryObjReader();

	const char* GetVertexBuffer();
	const char* GetUVBuffer();
	const char* GetNormalBuffer();
	unsigned int GetVertexCount();
	ImageBuffer* GetImageBuffer();

	unsigned int GetVertexBufferSize();
	unsigned int GetUVBufferSize();
	unsigned int GetNoralBufferSize();
};

#endif