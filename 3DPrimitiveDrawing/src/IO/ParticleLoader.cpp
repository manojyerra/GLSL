#include "ParticleLoader.h"
#include "GLBuffer.h"
#include "ShadersManager.h"
#include "Cam.h"
#include "FileReader.h"
#include "Input.h"
#include "GLMemoryTrace.h"
#include "GLMemory.h"

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
	
	_vertexBufferID = GLMemory::CreateBuffer(vertexBufLen, vertexBuf);
	
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

	_colorBufferID = GLMemory::CreateBuffer(colorBufLen, colorBuf);
	
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
	_vertexBufferID = GLCreateBuffer(length, (GLvoid*)fileData);
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

	_colorBufferID = GLCreateBuffer(colorBufLen, (GLvoid*)colorBuf);
	free(colorBuf);
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

	_lowPolyVertexBufferID = GLCreateBuffer(_lowPolyVertexCount * bpv, (GLvoid*)lowPolyBuf);
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

	_lowPolyColorBufferID = GLCreateBuffer(colorBufLen, (GLvoid*)colorBuf);
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
		GLDeleteBuffer(_vertexBufferID);
		_vertexBufferID = 0;
	}

	if (_lowPolyColorBufferID)
	{
		GLDeleteBuffer(_lowPolyColorBufferID);
		_lowPolyColorBufferID = 0;
	}

	if (_colorBufferID)
	{
		GLDeleteBuffer(_colorBufferID);
		_colorBufferID = 0;
	}

	if (_lowPolyVertexBufferID)
	{
		GLDeleteBuffer(_lowPolyVertexBufferID);
		_lowPolyVertexBufferID = 0;
	}
}
