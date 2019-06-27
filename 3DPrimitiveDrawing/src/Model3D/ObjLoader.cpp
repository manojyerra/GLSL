#include "ObjLoader.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "GLUtil.h"
#include "ShadersManager.h"
#include "GLBuffer.h"
#include "Vector3.h"
#include "Cam.h"

ObjLoader::ObjLoader(string folderPath)
{
	ReadObjFile(folderPath+"/objFile.obj");
	//LoadTextures(folderPath);

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram("shaders/NormalsAndMaterial/NormalsAndMaterial.vs",
		"shaders/NormalsAndMaterial/NormalsAndMaterial.fs");
}

void ObjLoader::ReadObjFile(string filePath)
{
	CFileReader fileReader(filePath.c_str(), "rb");
	
	char* line = NULL;

	float vx, vy, vz;
	float tx, ty;
	float nx, ny, nz;

	vector<CVector3> vertexVec;
	vector<CVector3> uvVec;
	vector<CVector3> normalVec;

	while( (line = fileReader.ReadLine())!= NULL)
	{
		if(line[0] == 'v' && line[1] == ' ')
		{
			sscanf(line, "v %f %f %f", &vx, &vy, &vz);
			vertexVec.push_back(CVector3(vx, vy, vz));
		}
		else if(line[0] == 'v' && line[1] == 't')
		{
			sscanf(line, "vt %f %f", &tx, &ty);
			uvVec.push_back(CVector3(tx, 1-ty, 0));
		}
		else if(line[0] == 'v' && line[1] == 'n')
		{
			sscanf(line, "vn %f %f %f", &nx, &ny, &nz);
			normalVec.push_back(CVector3(nx, ny, nz));
		}

		free(line);
	}

	fileReader.Reset();

	int v[3];
	int t[3];
	int n[3];
			
	for(int i=0; i<3; i++)
	{
		v[i] = 0;
		t[i] = 0;
		n[i] = 0;
	}

	bool uvsExist = uvVec.size() > 1;
	bool normalsExist = normalVec.size() > 1;

	GLBuffer* buffer = new GLBuffer(1024, false, uvsExist, normalsExist);

	buffer->glBegin(GL_TRIANGLES);


	while((line = fileReader.ReadLine())!= NULL)
	{
		if(line[0] == 'f' && line[1] == ' ')
		{
			if(uvsExist && normalsExist)
				sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v[0],&t[0],&n[0], &v[1],&t[1],&n[1], &v[2],&t[2],&n[2]);
			else if(uvsExist && !normalsExist)
				sscanf(line, "f %d/%d %d/%d %d/%d", &v[0],&t[0], &v[1],&t[1], &v[2],&t[2]);
			else if(!uvsExist && normalsExist)
				sscanf(line, "f %d//%d %d//%d %d//%d", &v[0],&n[0], &v[1],&n[1], &v[2],&n[2]);
			else if(!uvsExist && !normalsExist)
				sscanf(line, "f %d %d %d", &v[0], &v[1], &v[2]);

			for(int i=0; i<3; i++)
			{
				v[i]--;
				t[i]--;
				n[i]--;
			}

			for(int i=0; i<3; i++)
			{
				if(uvsExist)
				{
					buffer->glTexCoord2f(uvVec[t[i]]);
				}

				if(normalsExist)
				{
					buffer->glNormal3f(normalVec[n[i]]);	
				}

				buffer->glVertex3f(vertexVec[v[i]]);
			}
		}

		free(line);
	}

	buffer->glEnd();

	_vertexBufferID = buffer->GetVertexBufferID();
	_normalBufferID = buffer->GetNormalBufferID();
	_uvBufferID = buffer->GetUVBufferID();

	_vertexCount = buffer->GetVertexCount();

	delete buffer;
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

	GLfloat Ka[] = { 0.5f, 0.0f, 0.0f, 1.0f };
	GLfloat Kd[] = { 0.4f, 0.4f, 0.5f, 1.0f };
	GLfloat Ks[] = { 0.8f, 0.8f, 0.0f, 1.0f };
	GLfloat Se = 20.0f;

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