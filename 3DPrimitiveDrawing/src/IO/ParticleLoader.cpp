#include "ParticleLoader.h"
#include "GLBuffer.h"
#include "ShadersManager.h"
#include "Cam.h"
#include "FileReader.h"
#include "Input.h"

ParticleLoader::ParticleLoader()
{
	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram(
		"shaders/CubeGeo/CubeGeo.vs",
		"shaders/CubeGeo/CubeGeo.gs",
		"shaders/CubeGeo/CubeGeo.fs");

	_vertexBufferID = 0;
	_colorBufferID = 0;
	_vertexCount = 0;
	_cubeHalfLen = 1.0f;
	_methodNum = 1;
	
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


	_cubeHalfLen = 0.0015f;

	FILE* fp = fopen("data/xData.bin", "rb");

	if (fp)
	{
		unsigned int length = FileReader::GetLength("data/xData.bin");
		char* vertexBuf = (char*)malloc(length);
		fread(vertexBuf, 1, length, fp);
		fclose(fp);

		//char* halfVertexBuf = (char*)malloc(length / 2);
		//_vertexCount = 0;
		//for (int i = 0, j = 0; i < length-384; i += 384, j += 12)
		//{
		//	memcpy(&halfVertexBuf[j], &vertexBuf[i], 12);
		//	_vertexCount++;
		//}
		//free(vertexBuf);
		//vertexBuf = halfVertexBuf;

		_vertexCount = length / 12;

		glGenBuffers(1, &_vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, _vertexCount*12, vertexBuf, GL_STATIC_DRAW);
		free(vertexBuf);

		unsigned int colorBufLen = _vertexCount * 3;
		char* colorBuf = (char*)malloc(colorBufLen);

		for (unsigned int i = 0; i < _vertexCount; i++)
		{
			int ii = i * 3;

			colorBuf[ii + 0] = 128;
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
	if (Input::IsKeyTyped((int)'1'))
	{
		_methodNum = 1;
		printf("\nmethod 1");
	}
	else if (Input::IsKeyTyped((int)'2'))
	{
		_methodNum = 2;
		printf("\nmethod 2");
	}
	else if (Input::IsKeyTyped((int)'3'))
	{
		_methodNum = 3;
		printf("\nmethod 3");
	}

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
	_shaderProgram->SetUniform1f("hLen", _cubeHalfLen);

	glUniform1i(glGetUniformLocation(_shaderProgram->ProgramID(), "methodNum"), _methodNum);

	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint colorLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
	glEnableVertexAttribArray(colorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
	glVertexAttribPointer(colorLoc, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, (void*)0);

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
