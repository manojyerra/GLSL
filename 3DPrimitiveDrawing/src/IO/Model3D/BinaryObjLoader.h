#ifndef BinaryObjLoader_H
#define BinaryObjLoader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "FloatArray.h"
#include "ULongArray.h"
#include "VoidPtrArray.h"

#include <vector>
#include <string>
using namespace std;


class BinaryObjLoader
{
private:
	unsigned int _vertexCount;

	unsigned int _vertexBufferID;
	unsigned int _normalBufferID;
	unsigned int _uvBufferID;

	unsigned int _baseTexID;

	ShaderProgram* _shaderProgram;

	GLMat _modelMat;

	void ReadObjFile(string folderPath);
	void LoadTextures(string folderPath);

public:
	BinaryObjLoader(string folderPath);
	~BinaryObjLoader();

	void Draw();
};

#endif