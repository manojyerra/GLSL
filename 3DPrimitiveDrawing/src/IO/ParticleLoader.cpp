#include "ParticleLoader.h"
#include "GLBuffer.h"
#include "ShadersManager.h"
#include "Cam.h"
#include "FileReader.h"
#include "Input.h"
#include "GLMemoryTrace.h"

ParticleLoader::ParticleLoader()
{
	_cubeParticleShader = ShadersManager::GetInstance()->CreateShaderProgram(
		"shaders/CubeParticle/CubeParticle.vs",
		"shaders/CubeParticle/CubeParticle.gs",
		"shaders/CubeParticle/CubeParticle.fs");

	_vertexBufferID = 0;
	_colorBufferID = 0;
	_vertexCount = 0;

	_lowPolyVertexBufferID = 0;
	_lowPolyColorBufferID = 0;
	_lowPolyVertexCount = 0;

	_cubeHalfLen = 1.0f;
	_methodNum = 2;
	
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

	long startTime = GetTickCount();

	FILE* fp = fopen("data/xData.bin", "rb");

	if (fp)
	{
		unsigned int fileLen = FileReader::GetLength("data/xData.bin");
		char* fileData = (char*)malloc(fileLen);
		fread(fileData, 1, fileLen, fp);
		fclose(fp);

		LoadData(fileData, fileLen);
		LoadLowPolyData(fileData, fileLen);

		free(fileData);
	}

	long timeTaken = GetTickCount() - startTime;
}

void ParticleLoader::LoadData(const char* fileData, unsigned int length)
{
	long startTime = GetTickCount();

	glGenBuffers(1, &_vertexBufferID);
	__glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	__glBufferData(GL_ARRAY_BUFFER, length, fileData, GL_STATIC_DRAW);

 	_vertexCount = length / BYTES_PER_VERTEX;

	unsigned int colorBufLen = _vertexCount * 3;
	char* colorBuf = (char*)malloc(colorBufLen);

	for (unsigned int i = 0; i < _vertexCount; i++)
	{
		int ii = i * 3;

		colorBuf[ii + 0] = 200;
		colorBuf[ii + 1] = 0;
		colorBuf[ii + 2] = 0;
	}

	glGenBuffers(1, &_colorBufferID);
	__glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
	__glBufferData(GL_ARRAY_BUFFER, colorBufLen, colorBuf, GL_STATIC_DRAW);
	free(colorBuf);

	long timeTaken = GetTickCount() - startTime;
}


void ParticleLoader::LoadLowPolyData(const char* fileData, unsigned int length)
{
	int bpv = BYTES_PER_VERTEX;
	int skipNumVertex = 25;
	int skipBytes = skipNumVertex * bpv;
	unsigned int bufLen = length / (skipNumVertex - 1);

	char* lowPolyBuf = (char*)malloc(bufLen);
	
	_lowPolyVertexCount = 0;
	
	for (int i = 0, j = 0; i < length- skipBytes; i += skipBytes, j += bpv)
	{
		memcpy(&lowPolyBuf[j], &fileData[i], bpv);
		_lowPolyVertexCount++;
	}

	glGenBuffers(1, &_lowPolyVertexBufferID);
	__glBindBuffer(GL_ARRAY_BUFFER, _lowPolyVertexBufferID);
	__glBufferData(GL_ARRAY_BUFFER, _lowPolyVertexCount * bpv, lowPolyBuf, GL_STATIC_DRAW);
	free(lowPolyBuf);

	unsigned int colorBufLen = _lowPolyVertexCount * 3;
	char* colorBuf = (char*)malloc(colorBufLen);

	for (unsigned int i = 0; i < _lowPolyVertexCount; i++)
	{
		int ii = i * 3;

		colorBuf[ii + 0] = 200;
		colorBuf[ii + 1] = 0;
		colorBuf[ii + 2] = 0;
	}

	glGenBuffers(1, &_lowPolyColorBufferID);
	__glBindBuffer(GL_ARRAY_BUFFER, _lowPolyColorBufferID);
	__glBufferData(GL_ARRAY_BUFFER, colorBufLen, colorBuf, GL_STATIC_DRAW);
	free(colorBuf);
}

void ParticleLoader::DrawAllParticles()
{
	_cubeParticleShader->Begin();

	glm::mat4 projMat = glm::make_mat4(Cam::GetInstance()->projMat.m);
	glm::mat4 viewMat = glm::make_mat4(Cam::GetInstance()->viewMat.m);
	glm::mat4 modelMat = glm::make_mat4(_modelMat.m);

	glm::mat4 mvp = projMat * viewMat * modelMat;
	glm::mat4 modelViewMat = viewMat * modelMat;
	glm::mat3 normalMat = glm::mat3(viewMat);

	_cubeParticleShader->SetUniformMatrix4fv("mvp", glm::value_ptr(mvp));
	_cubeParticleShader->SetUniformMatrix3fv("normalMat", glm::value_ptr(normalMat));
	_cubeParticleShader->SetUniformMatrix4fv("modelViewMat", glm::value_ptr(modelViewMat));
	_cubeParticleShader->SetUniform1f("hLen", _cubeHalfLen);
	_cubeParticleShader->SetUniform1i("methodNum", _methodNum);

	GLuint vertexLoc = glGetAttribLocation(_cubeParticleShader->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint colorLoc = glGetAttribLocation(_cubeParticleShader->ProgramID(), "color");
	glEnableVertexAttribArray(colorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
	glVertexAttribPointer(colorLoc, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_POINTS, 0, _vertexCount);

	glDisableVertexAttribArray(vertexLoc);
	glDisableVertexAttribArray(colorLoc);

	_cubeParticleShader->End();
}

void ParticleLoader::DrawLowPolyParticles()
{
	_cubeParticleShader->Begin();

	glm::mat4 projMat = glm::make_mat4(Cam::GetInstance()->projMat.m);
	glm::mat4 viewMat = glm::make_mat4(Cam::GetInstance()->viewMat.m);
	glm::mat4 modelMat = glm::make_mat4(_modelMat.m);

	glm::mat4 mvp = projMat * viewMat * modelMat;
	glm::mat4 modelViewMat = viewMat * modelMat;
	glm::mat3 normalMat = glm::mat3(viewMat);

	_cubeParticleShader->SetUniformMatrix4fv("mvp", glm::value_ptr(mvp));
	_cubeParticleShader->SetUniformMatrix3fv("normalMat", glm::value_ptr(normalMat));
	_cubeParticleShader->SetUniformMatrix4fv("modelViewMat", glm::value_ptr(modelViewMat));
	_cubeParticleShader->SetUniform1f("hLen", _cubeHalfLen);
	_cubeParticleShader->SetUniform1i("methodNum", _methodNum);

	GLuint vertexLoc = glGetAttribLocation(_cubeParticleShader->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _lowPolyVertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint colorLoc = glGetAttribLocation(_cubeParticleShader->ProgramID(), "color");
	glEnableVertexAttribArray(colorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _lowPolyColorBufferID);
	glVertexAttribPointer(colorLoc, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_POINTS, 0, _lowPolyVertexCount);

	glDisableVertexAttribArray(vertexLoc);
	glDisableVertexAttribArray(colorLoc);

	_cubeParticleShader->End();
}

ParticleLoader::~ParticleLoader()
{
	if(_cubeParticleShader)
	{
		ShadersManager::GetInstance()->DeleteShaderProgram(_cubeParticleShader);
		_cubeParticleShader = NULL;
	}

	if (_vertexBufferID)
	{
		__glDeleteBuffers(1, &_vertexBufferID);
		_vertexBufferID = 0;
	}

	if (_lowPolyColorBufferID)
	{
		__glDeleteBuffers(1, &_lowPolyColorBufferID);
		_lowPolyColorBufferID = 0;
	}

	if (_colorBufferID)
	{
		__glDeleteBuffers(1, &_colorBufferID);
		_colorBufferID = 0;
	}

	if (_lowPolyVertexBufferID)
	{
		__glDeleteBuffers(1, &_lowPolyVertexBufferID);
		_lowPolyVertexBufferID = 0;
	}
}


//if (Input::IsKeyTyped((int)'1'))
//{
//	_methodNum = 1;
//	printf("\nmethod 1");
//}
//else if (Input::IsKeyTyped((int)'2'))
//{
//	_methodNum = 2;
//	printf("\nmethod 2");
//}
//else if (Input::IsKeyTyped((int)'3'))
//{
//	_methodNum = 3;
//	printf("\nmethod 3");
//}


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
