#include "ParticleLoader.h"
#include "GLBuffer.h"
#include "ShadersManager.h"
#include "Cam.h"
#include "CFileReader.h"
#include <set>
#include <unordered_set>
using namespace std;

ParticleLoader::ParticleLoader(CVector3& v1, CVector3& v2, CVector3& v3)
{
	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram(
		"shaders/SingleColor/SingleColor.vs", 
		"shaders/SingleColor/SingleColor.fs");


	_vertexBufferID = 0;
	_vertexCount = 0;

	FILE* fp = fopen("data/xData.bin", "rb");

	if (fp)
	{
		unsigned int length = CFileReader::GetLength("data/xData.bin");
		char* buf = (char*)malloc(length);
		
		fread(buf, 1, length, fp);
		fclose(fp);

		glGenBuffers(1, &_vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, length, buf, GL_STATIC_DRAW);

		_vertexCount = length / 12;

		char arr[64];
		unordered_set<Tuple, MyHashFunction> dupRemSet;
		float* values = (float*)buf;
		unsigned int numFloats = length / 4;

		for (int i = 0; i < numFloats; i += 3)
		{
			float x = values[i + 0];
			float y = values[i + 1];
			float z = values[i + 2];

			//x = (float)(((int)(x * 1000)) / 1000.0f);
			//y = (float)(((int)(y * 1000)) / 1000.0f);
			//z = (float)(((int)(z * 1000)) / 1000.0f);

			dupRemSet.insert(Tuple(x, y, z));
		}

		int setSize = dupRemSet.size();

		free(buf);
	}
}

void ParticleLoader::Draw()
{
	_shaderProgram->Begin();

	GLint projMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "projMat");
	GLint viewMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "viewMat");
	GLint modelMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "modelMat");

	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, Cam::GetInstance()->projMat.m);
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, Cam::GetInstance()->viewMat.m);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, _modelMat.m);

	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_POINTS, 0, _vertexCount);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(vertexLoc);

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