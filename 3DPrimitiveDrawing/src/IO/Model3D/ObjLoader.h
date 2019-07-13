#ifndef ObjLoader_H
#define ObjLoader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "FloatArray.h"
#include "ULongArray.h"
#include "VoidPtrArray.h"

#include <vector>
#include <string>
using namespace std;


class ObjLoader
{
private:
	unsigned int _vertexCount;

	unsigned int _vertexBufferID;
	unsigned int _normalBufferID;
	unsigned int _uvBufferID;

	unsigned int _baseTexID;

	ShaderProgram* _shaderProgram;

	GLMat _modelMat;
	bool _writeBinaryToFile;

	void ReadObjFile(string folderPath);
	void LoadTextures(string folderPath);

	void Init(string filePath, bool writeBinaryToFile);
	void WriteBinaryToFile(string folderPath,
		FloatArray& vertexFloatArr, FloatArray& uvFloatArr, FloatArray& normalFloatArr);

public:
	ObjLoader(string folderPath);
	ObjLoader(string folderPath, bool writeBinaryToFile);

    void Draw();

	~ObjLoader();
};

#endif