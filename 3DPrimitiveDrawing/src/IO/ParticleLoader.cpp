#include "ParticleLoader.h"
#include "GLBatch.h"
#include "ShadersManager.h"
#include "Cam.h"
#include "FileReader.h"
#include "GLMemory.h"

ParticleLoader::ParticleLoader()
{
	_shader = new CubeGeometryShader();

	_vertexBufferID = 0;
	_colorBufferID = 0;
	_vertexCount = 0;

	_lowPolyVertexBufferID = 0;
	_lowPolyColorBufferID = 0;
	_lowPolyVertexCount = 0;

	_modelMat.SetRotation(glm::vec3(0,90,90));
	
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
	
	_vertexBufferID = GLCreateBuffer(vertexBufLen, vertexBuf);
	
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

	_colorBufferID = GLCreateBuffer(colorBufLen, colorBuf);
	
	_vertexCount = vertexBufLen / 12;
	free(vertexBuf);
	*/
	
	FILE* fp = fopen("data/demo/xData.bin", "rb");

	if (fp)
	{
		unsigned int fileLen = FileReader::GetLength("data/demo/xData.bin");
		char* fileData = (char*)malloc(fileLen);
		fread(fileData, 1, fileLen, fp);
		fclose(fp);

		LoadData(fileData, fileLen);
		LoadLowPolyData(fileData, fileLen);

		free(fileData);
	}
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
	int skipNumVertex = 50;
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
	memset(colorBuf, 0, colorBufLen);

	for (unsigned int i = 0; i < _lowPolyVertexCount; i++)
	{
		int ii = i * 3;

		colorBuf[ii + 0] = 200;
		//colorBuf[ii + 1] = 0;
		//colorBuf[ii + 2] = 0;
	}

	_lowPolyColorBufferID = GLCreateBuffer(colorBufLen, (GLvoid*)colorBuf);
	free(colorBuf);
}

void ParticleLoader::SetPosition(float x, float y, float z)
{
	_modelMat.SetPos(x, y, z);
}

void ParticleLoader::DrawAllParticles()
{
	_shader->SetModelMatrix(_modelMat.m);
	_shader->SetVertexBufferID(_vertexBufferID);
	_shader->SetColorBufferID(_colorBufferID);

	_shader->Begin();
	_shader->SetUniformsAndAttributes();
	glDrawArrays(GL_POINTS, 0, _vertexCount);
	_shader->End();
}

void ParticleLoader::DrawLowPolyParticles()
{
	_shader->SetModelMatrix(_modelMat.m);
	_shader->SetVertexBufferID(_lowPolyVertexBufferID);
	_shader->SetColorBufferID(_lowPolyColorBufferID);

	_shader->Begin();
	_shader->SetUniformsAndAttributes();
	glDrawArrays(GL_POINTS, 0, _lowPolyVertexCount);
	_shader->End();
}

ParticleLoader::~ParticleLoader()
{
	if(_shader)
	{
		delete _shader;
		_shader = NULL;
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
