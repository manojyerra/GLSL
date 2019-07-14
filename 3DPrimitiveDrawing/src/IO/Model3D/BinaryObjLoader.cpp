#include "BinaryObjLoader.h"
#include "FileReader.h"
#include "ImageBuffer.h"
#include "GLUtil.h"
#include "ShadersManager.h"
#include "GLBuffer.h"
#include "glm/glm.hpp"
#include "Cam.h"
#include "UtilFuncs.h"
#include "GLMemoryTrace.h"


BinaryObjLoader::BinaryObjLoader(string folderPath)
{
	_vertexCount = 0;

	_vertexBufferID = 0;
	_normalBufferID = 0;
	_uvBufferID = 0;

	ReadObjFile(folderPath);
	LoadTextures(folderPath);

	GLfloat Ka[] = { 0.05375,	0.05,	0.06625,	0.82 };
	GLfloat Kd[] = { 0.18275,	0.17,	0.22525,	0.82 };
	GLfloat Ks[] = { 0.332741,	0.328634,	0.346435,	0.82 };
	GLfloat Se = 38.4;

	_phongShader = new PhongShader(PhongShader::PER_PIXEL_SHADER);
	_phongShader->SetLightPos(0.0, 0.0, 0.0);
	_phongShader->SetAmbientColor(Ka[0], Ka[1], Ka[2], Ka[3]);
	_phongShader->SetDiffuseColor(Kd[0], Kd[1], Kd[2], Kd[3]);
	_phongShader->SetSpecularColor(Ks[0], Ks[1], Ks[2], Ks[3]);
	_phongShader->SetShininess(Se);

	_phongShader->SetVertexBufferID(_vertexBufferID);
	_phongShader->SetNormalBufferID(_normalBufferID);
}

void BinaryObjLoader::ReadObjFile(string folderPath)
{
	unsigned int startTime = GetTickCount();

	if (FileReader::IsFileExists(folderPath + "/vertex.buf"))
	{
		FileReader* fileReader = new FileReader(folderPath + "/vertex.buf", "rb");
		glGenBuffers(1, &_vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
		__glBufferData(_vertexBufferID, GL_ARRAY_BUFFER, fileReader->GetLength(), fileReader->GetData(), GL_STATIC_DRAW);
		_vertexCount = fileReader->GetLength() / 12;
		delete fileReader;
	}

	if (FileReader::IsFileExists(folderPath + "/normal.buf"))
	{
		FileReader* fileReader = new FileReader(folderPath + "/normal.buf", "rb");
		glGenBuffers(1, &_normalBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _normalBufferID);
		__glBufferData(_normalBufferID, GL_ARRAY_BUFFER, fileReader->GetLength(), fileReader->GetData(), GL_STATIC_DRAW);
		delete fileReader;
	}

	if (FileReader::IsFileExists(folderPath + "/uv.buf"))
	{
		FileReader* fileReader = new FileReader(folderPath + "/uv.buf", "rb");
		glGenBuffers(1, &_uvBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
		__glBufferData(_uvBufferID, GL_ARRAY_BUFFER, fileReader->GetLength(), fileReader->GetData(), GL_STATIC_DRAW);
		delete fileReader;
	}
}

void BinaryObjLoader::Draw()
{
	_phongShader->Begin();
	_phongShader->SetModelMatrix(_modelMat.m);
	_phongShader->SetUniformsAndAttributes();

	glDrawArrays(GL_TRIANGLES, 0, _vertexCount);

	_phongShader->End();
}


void BinaryObjLoader::LoadTextures(string folderPath)
{
	string filePath = folderPath + "/texture.png";

	if (FileReader::IsFileExists(filePath))
	{
		ImageBuffer* imgBuf = new ImageBuffer(filePath);

		_baseTexID = GLUtil::GenerateGLTextureID(imgBuf->GetWidth(), imgBuf->GetHeight(),
			imgBuf->GetBytesPerPixel(), imgBuf->GetBuffer());

		delete imgBuf;
	}
}

BinaryObjLoader::~BinaryObjLoader()
{
	if (_phongShader)
	{
		delete _phongShader;
		_phongShader = NULL;
	}

	if (_vertexBufferID)
	{
		__glDeleteBuffers(1, &_vertexBufferID);
		_vertexBufferID = 0;
	}

	if (_normalBufferID)
	{
		__glDeleteBuffers(1, &_normalBufferID);
		_normalBufferID = 0;
	}

	if (_uvBufferID)
	{
		__glDeleteBuffers(1, &_uvBufferID);
		_uvBufferID = 0;
	}
}


//GLfloat Ka[] = { 1.0f, 0.5f, 0.5f, 1.0f };
//GLfloat Kd[] = { 1.0f, 0.1f, 0.1f, 1.0f };
//GLfloat Ks[] = { 1.0f, 1.0f, 1.0f, 1.0f };

//GLfloat Ka[] = { 0.329412,	0.223529,	0.027451,	1 };
//GLfloat Kd[] = { 0.780392,	0.568627,	0.113725,	1 };
//GLfloat Ks[] = { 0.992157,	0.941176,	0.807843,	1 };
//GLfloat Se = 27.8974;

//GLfloat Ka[] = { 0.19225,	0.19225,	0.19225,	1 };
//GLfloat Kd[] = { 0.50754,	0.50754,	0.50754,	1 };
//GLfloat Ks[] = { 0.508273,	0.508273,	0.508273,	1 };
//GLfloat Se = 51.2;

//GLfloat Ka[] = { 0.5f, 0.0f, 0.0f, 1.0f };
//GLfloat Kd[] = { 0.4f, 0.4f, 0.5f, 1.0f };
//GLfloat Ks[] = { 0.8f, 0.8f, 0.0f, 1.0f };
//GLfloat Se = 25.0f;