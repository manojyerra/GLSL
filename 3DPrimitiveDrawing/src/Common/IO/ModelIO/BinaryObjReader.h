#ifndef BinaryObjReader_H
#define BinaryObjReader_H

#include "FloatArray.h"

#include <vector>
#include <string>
using namespace std;


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

	const float* GetVertexArr();
	const float* GetUVArr();
	const float* GetNormalArr();
	unsigned int GetVertexCount();
	ImageBuffer* GetImageBuffer();
};

#endif