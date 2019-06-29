#include "ObjLoader.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "GLUtil.h"
#include "ShadersManager.h"
#include "GLBuffer.h"
#include "Vector3.h"
#include "Cam.h"
#include "UtilFuncs.h"
#include "FloatArray.h"
#include "ULongArray.h"
#include "VoidPtrArray.h"

ObjLoader::ObjLoader(string folderPath)
{
	_vertexCount = 0;
	
	_vertexBufferID = 0;
	_normalBufferID = 0;
	_uvBufferID = 0;

	ReadObjFile(folderPath+"/objFile.obj");
	//LoadTextures(folderPath);

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram("shaders/NormalsAndMaterial/NormalsAndMaterial.vs",
		"shaders/NormalsAndMaterial/NormalsAndMaterial.fs");
}

void ObjLoader::ReadObjFile(string filePath)
{
	unsigned int startTime = GetTickCount();

	CFileReader fileReader(filePath, "rb");

	float vx, vy, vz;
	float tx, ty;
	float nx, ny, nz;

	vector<CVector3> vertexVec;
	vector<CVector3> uvVec;
	vector<CVector3> normalVec;

	VoidPtrArray facesArr(1024 * 1024);

	char* line = NULL;

	int charSize = sizeof(char*);
	unsigned long tempLong = 2111463936;
	unsigned char* tempChar = (unsigned char*)intptr_t(tempLong);
	unsigned long temp2 = (unsigned long)tempChar;
	char* tempChar2 = (char*)temp2;

	while ((line = fileReader.ReadLine()) != NULL)
	{
		if (line[0] == 'v' && line[1] == ' ')
		{
			UtilFuncs::scan_vertex(line, &vx, &vy, &vz);
			vertexVec.push_back(CVector3(vx, vy, vz));
			free(line);
		}
		else if (line[0] == 'v' && line[1] == 't')
		{
			UtilFuncs::scan_uv(line, &tx, &ty);
			uvVec.push_back(CVector3(tx, 1 - ty, 0));
			free(line);
		}
		else if (line[0] == 'v' && line[1] == 'n')
		{
			UtilFuncs::scan_normal(line, &nx, &ny, &nz);
			normalVec.push_back(CVector3(nx, ny, nz));
			free(line);
		}
		else if (line[0] == 'f' && line[1] == ' ')
		{
			facesArr.push_back(line);
		}
	}

	int v[3];
	int t[3];
	int n[3];

	for (int i = 0; i < 3; i++)
	{
		v[i] = 0;
		t[i] = 0;
		n[i] = 0;
	}

	bool uvsExist = uvVec.size() > 1;
	bool normalsExist = normalVec.size() > 1;

	unsigned int initSize = 1024 * 1024;

	FloatArray vertexFloatArr(initSize * 4);
	FloatArray uvFloatArr(initSize * 3);
	FloatArray normalFloatArr(initSize * 4);

	//unsigned int fileParseStartTime = GetTickCount();

	unsigned int facesStrArrSize = facesArr.size();
	void** facesStrArrPtr = facesArr.getArray();

	if (uvsExist && normalsExist)
	{
		for (int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			UtilFuncs::scanFace_VTN(line, &v[0], &t[0], &n[0], &v[1], &t[1], &n[1], &v[2], &t[2], &n[2]);

			for (int i = 0; i < 3; i++)
			{
				vertexFloatArr.push_back_3(vertexVec[v[i] - 1]);
				uvFloatArr.push_back_2(uvVec[t[i] - 1]);
				normalFloatArr.push_back_3(normalVec[n[i] - 1]);
			}

			free(line);
		}
	}
	else if (uvsExist && !normalsExist)
	{
		for (int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			UtilFuncs::scanFace_VT(line, &v[0], &t[0], &v[1], &t[1], &v[2], &t[2]);

			for (int i = 0; i < 3; i++)
			{
				vertexFloatArr.push_back_3(vertexVec[v[i] - 1]);
				uvFloatArr.push_back_2(uvVec[t[i] - 1]);
			}

			free(line);
		}
	}
	else if (!uvsExist && normalsExist)
	{
		for (int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			UtilFuncs::scanFace_VN(line, &v[0], &n[0], &v[1], &n[1], &v[2], &n[2]);

			for (int i = 0; i < 3; i++)
			{
				vertexFloatArr.push_back_3(vertexVec[v[i] - 1]);
				normalFloatArr.push_back_3(normalVec[n[i] - 1]);
			}

			free(line);
		}
	}
	else if (!uvsExist && !normalsExist)
	{
		for (int arrIndex = 0; arrIndex < facesStrArrSize; arrIndex++)
		{
			char* line = (char*)facesStrArrPtr[arrIndex];

			UtilFuncs::scanFace_V(line, &v[0], &v[1], &v[2]);

			for (int i = 0; i < 3; i++)
			{
				vertexFloatArr.push_back_3(vertexVec[v[i] - 1]);
			}

			free(line);
		}
	}

	glGenBuffers(1, &_vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexFloatArr.size() * 4, vertexFloatArr.getArray(), GL_STATIC_DRAW);

	if (uvFloatArr.size() > 0)
	{
		glGenBuffers(1, &_uvBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
		glBufferData(GL_ARRAY_BUFFER, uvFloatArr.size() * 4, uvFloatArr.getArray(), GL_STATIC_DRAW);
	}

	if (normalFloatArr.size() > 0)
	{
		glGenBuffers(1, &_normalBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, _normalBufferID);
		glBufferData(GL_ARRAY_BUFFER, normalFloatArr.size() * 4, normalFloatArr.getArray(), GL_STATIC_DRAW);
	}

	//_vertexBufferID = buffer->GetVertexBufferID();
	//_normalBufferID = buffer->GetNormalBufferID();
	//_uvBufferID = buffer->GetUVBufferID();

	_vertexCount = vertexFloatArr.size() / 3;
}

void ObjLoader::Draw()
{
	_shaderProgram->Begin();

	GLint projMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "projMat");
	GLint modelMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "modelMat");
	GLint normalMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "normalMat");
	GLint oriMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "oriMat");

	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, Cam::GetInstance()->projMat.m);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, Cam::GetInstance()->modelMat.m);
	glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, Cam::GetInstance()->normalMat);
	glUniformMatrix4fv(oriMatLoc, 1, GL_FALSE, _oriMat.m);
	
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

	GLfloat Ka[] = { 0.05375,	0.05,	0.06625,	0.82 };
	GLfloat Kd[] = { 0.18275,	0.17,	0.22525,	0.82 };
	GLfloat Ks[] = { 0.332741,	0.328634,	0.346435,	0.82 };
	GLfloat Se = 38.4;


	//GLfloat Ka[] = { 0.5f, 0.0f, 0.0f, 1.0f };
	//GLfloat Kd[] = { 0.4f, 0.4f, 0.5f, 1.0f };
	//GLfloat Ks[] = { 0.8f, 0.8f, 0.0f, 1.0f };
	//GLfloat Se = 25.0f;

	glUniform3f(glGetUniformLocation(_shaderProgram->ProgramID(), "lightPos"), 0.0, 0.0, 0.0);
	glUniform4f(glGetUniformLocation(_shaderProgram->ProgramID(), "ambient"), Ka[0], Ka[1], Ka[2], Ka[3]);
	glUniform4f(glGetUniformLocation(_shaderProgram->ProgramID(), "diffuse"), Kd[0], Kd[1], Kd[2], Kd[3]);
	glUniform4f(glGetUniformLocation(_shaderProgram->ProgramID(), "specular"), Ks[0], Ks[1], Ks[2], Ks[3]);
	glUniform1f(glGetUniformLocation(_shaderProgram->ProgramID(), "shininess"), Se);

	GLuint normalID = glGetAttribLocation(_shaderProgram->ProgramID(), "normal");
	glEnableVertexAttribArray(normalID);
	glBindBuffer(GL_ARRAY_BUFFER, _normalBufferID);
	glVertexAttribPointer(normalID, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);

	GLuint vertexID = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexID);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, _vertexCount);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(vertexID);

	_shaderProgram->End();
}


void ObjLoader::LoadTextures(string folderPath)
{
	ImageBuffer* imgBuf = new ImageBuffer(folderPath + "/texture.png");

	_baseTexID = GLUtil::GenerateGLTextureID(imgBuf->GetWidth(), imgBuf->GetHeight(),
		imgBuf->GetBytesPerPixel(), imgBuf->GetBuffer());

	delete imgBuf;
}