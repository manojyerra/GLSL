#include "ParticleLoader.h"
#include "GLBuffer.h"
#include "ShadersManager.h"
#include "Cam.h"
#include "FileReader.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "glm/ext.hpp"

#include <set>
#include <unordered_set>
using namespace std;

ParticleLoader::ParticleLoader()
{
	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram(
		"shaders/CubeGeo/CubeGeo.vs",
		"shaders/CubeGeo/CubeGeo.gs",
		"shaders/CubeGeo/CubeGeo.fs");

	_vertexBufferID = 0;
	_colorBufferID = 0;
	_vertexCount = 0;

	/*
	unsigned int vertexBufLen = 36;
	
	float* vertexBuf = (float*)malloc(vertexBufLen);
	
	float y = 0.0f;
	
	vertexBuf[0] = 0.0f;
	vertexBuf[1] = y;
	vertexBuf[2] = 0.0f;
	
	vertexBuf[3] = -5.0f;
	vertexBuf[4] = y;
	vertexBuf[5] = 0.0f;
	
	vertexBuf[6] = 5.0f;
	vertexBuf[7] = y;
	vertexBuf[8] = 0.0f;
	
	glGenBuffers(1, &_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexBufLen, vertexBuf, GL_STATIC_DRAW);
	
	
	unsigned int colorBufLen = 12;
	unsigned char* colorBuf = (unsigned char*)malloc(colorBufLen);
	
	colorBuf[0] = 255;
	colorBuf[1] = 0;
	colorBuf[2] = 0;
	
	colorBuf[3] = 0;
	colorBuf[4] = 255;
	colorBuf[5] = 0;
	
	colorBuf[6] = 0;
	colorBuf[7] = 0;
	colorBuf[8] = 255;
	
	glGenBuffers(1, &_colorBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
	glBufferData(GL_ARRAY_BUFFER, colorBufLen, colorBuf, GL_STATIC_DRAW);
	
	
	_vertexCount = vertexBufLen / 12;
	free(vertexBuf);
	*/


	FILE* fp = fopen("data/xData.bin", "rb");

	if (fp)
	{
		unsigned int length = FileReader::GetLength("data/xData.bin");
		char* vertexBuf = (char*)malloc(length);

		fread(vertexBuf, 1, length, fp);
		fclose(fp);
		_vertexCount = length / 12;

		glGenBuffers(1, &_vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, length, vertexBuf, GL_STATIC_DRAW);
		free(vertexBuf);

		unsigned int colorBufLen = _vertexCount * 3;
		char* colorBuf = (char*)malloc(colorBufLen);

		for (unsigned int i = 0; i < _vertexCount; i++)
		{
			int ii = i * 3;

			colorBuf[ii + 0] = 255;
			colorBuf[ii + 1] = 0;
			colorBuf[ii + 2] = 0;
		}

		glGenBuffers(1, &_colorBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
		glBufferData(GL_ARRAY_BUFFER, colorBufLen, colorBuf, GL_STATIC_DRAW);
		free(colorBuf);
	}
}

void ParticleLoader::Draw()
{
	_shaderProgram->Begin();

	glm::mat4 projMat = glm::make_mat4(Cam::GetInstance()->projMat.m);
	glm::mat4 viewMat = glm::make_mat4(Cam::GetInstance()->viewMat.m);
	glm::mat4 modelMat = glm::make_mat4(_modelMat.m);

	glm::mat4 mvp = projMat * viewMat * modelMat;
	glm::mat4 modelViewMat = viewMat * modelMat;
	glm::mat3 normalMat = glm::mat3(viewMat);

	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(mvp));
	_shaderProgram->SetUniformMatrix3fv("normalMat", glm::value_ptr(normalMat));
	_shaderProgram->SetUniformMatrix4fv("modelViewMat", glm::value_ptr(modelViewMat));
	_shaderProgram->SetUniform1f("hLen", 1.0);

	GLfloat Ka[] = { 0.329412,	0.223529,	0.027451,	1 };
	GLfloat Kd[] = { 0.780392,	0.568627,	0.113725,	1 };

	glUniform4f(glGetUniformLocation(_shaderProgram->ProgramID(), "ambient"), Ka[0], Ka[1], Ka[2], Ka[3]);
	glUniform4f(glGetUniformLocation(_shaderProgram->ProgramID(), "diffuse"), Kd[0], Kd[1], Kd[2], Kd[3]);

	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint colorLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
	glEnableVertexAttribArray(colorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
	glVertexAttribPointer(colorLoc, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

	glDrawArrays(GL_POINTS, 0, _vertexCount);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(vertexLoc);
	glDisableVertexAttribArray(colorLoc);

	_shaderProgram->End();
}

ParticleLoader::~ParticleLoader()
{
	if(_shaderProgram)
	{
		ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}



//ParticleLoader::ParticleLoader()
//{
//	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram(
//		"shaders/SingleColor/SingleColor.vs",
//		"shaders/SingleColor/SingleColor.fs");
//
//
//	_vertexBufferID = 0;
//	_vertexCount = 0;
//
//	FILE* fp = fopen("data/xData.bin", "rb");
//
//	if (fp)
//	{
//		unsigned int length = CFileReader::GetLength("data/xData.bin");
//		char* buf = (char*)malloc(length);
//
//		fread(buf, 1, length, fp);
//		fclose(fp);
//
//		glGenBuffers(1, &_vertexBufferID);
//		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
//		glBufferData(GL_ARRAY_BUFFER, length, buf, GL_STATIC_DRAW);
//
//		_vertexCount = length / 12;
//
//		char arr[64];
//		unordered_set<Tuple, MyHashFunction> dupRemSet;
//		float* values = (float*)buf;
//		unsigned int numFloats = length / 4;
//
//		for (int i = 0; i < numFloats; i += 3)
//		{
//			float x = values[i + 0];
//			float y = values[i + 1];
//			float z = values[i + 2];
//
//			//x = (float)(((int)(x * 1000)) / 1000.0f);
//			//y = (float)(((int)(y * 1000)) / 1000.0f);
//			//z = (float)(((int)(z * 1000)) / 1000.0f);
//
//			//dupRemSet.insert(Tuple(x, y, z));
//		}
//
//		//int setSize = dupRemSet.size();
//
//		free(buf);
//	}
//}



//
//ParticleLoader::ParticleLoader()
//{
//	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram(
//		"shaders/CubeGeo/CubeGeo.vs",
//		"shaders/CubeGeo/CubeGeo.gs",
//		"shaders/CubeGeo/CubeGeo.fs");
//
//	_vertexBufferID = 0;
//	_colorBufferID = 0;
//	_vertexCount = 0;
//
//	unsigned int vertexBufLen = 36;
//
//	float* vertexBuf = (float*)malloc(vertexBufLen);
//
//	float y = 0.0f;
//
//	vertexBuf[0] = 0.0f;
//	vertexBuf[1] = y;
//	vertexBuf[2] = 0.0f;
//
//	vertexBuf[3] = -5.0f;
//	vertexBuf[4] = y;
//	vertexBuf[5] = 0.0f;
//
//	vertexBuf[6] = 5.0f;
//	vertexBuf[7] = y;
//	vertexBuf[8] = 0.0f;
//
//	glGenBuffers(1, &_vertexBufferID);
//	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
//	glBufferData(GL_ARRAY_BUFFER, vertexBufLen, vertexBuf, GL_STATIC_DRAW);
//
//
//	unsigned int colorBufLen = 12;
//	unsigned char* colorBuf = (unsigned char*)malloc(colorBufLen);
//
//	colorBuf[0] = 255;
//	colorBuf[1] = 0;
//	colorBuf[2] = 0;
//
//	colorBuf[3] = 0;
//	colorBuf[4] = 255;
//	colorBuf[5] = 0;
//
//	colorBuf[6] = 0;
//	colorBuf[7] = 0;
//	colorBuf[8] = 255;
//
//	glGenBuffers(1, &_colorBufferID);
//	glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
//	glBufferData(GL_ARRAY_BUFFER, colorBufLen, colorBuf, GL_STATIC_DRAW);
//
//
//	_vertexCount = vertexBufLen / 12;
//	free(vertexBuf);
//}
