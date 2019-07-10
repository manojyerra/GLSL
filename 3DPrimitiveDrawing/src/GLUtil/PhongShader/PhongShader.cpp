#include "PhongShader.h"
#include "CFileReader.h"
#include "ImageBuffer.h"
#include "GLUtil.h"
#include "ShadersManager.h"
#include "GLBuffer.h"
#include "Vector3.h"
#include "Cam.h"
#include "UtilFuncs.h"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "glm/ext.hpp"

PhongShader::PhongShader()
{
	_vertexBufferID = 0;
	_normalBufferID = 0;
	_uvBufferID = 0;
	_baseTexID = 0;

	Ka[0] = Ka[1] = Ka[2] = Ka[3] = 0.0f;
	Kd[0] = Kd[1] = Kd[2] = Kd[3] = 0.0f;
	Ks[0] = Ks[1] = Ks[2] = Ks[3] = 0.0f;
	Se = 0.0f;

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram("shaders/Phong/Phong.vs", "shaders/Phong/Phong.fs");
}

void PhongShader::SetVertexBufferID(unsigned int bufferID)
{
	_vertexBufferID = bufferID;
}

void PhongShader::SetNormalBufferID(unsigned int bufferID)
{
	_normalBufferID = bufferID;
}

void PhongShader::SetUVBufferID(unsigned int bufferID)
{
	_uvBufferID = bufferID;
}

void PhongShader::SetBaseTexID(unsigned int texID)
{
	_baseTexID = texID;
}

void PhongShader::SetLightPos(CVector3& vec)
{
	lightPos.x = vec.x;
	lightPos.y = vec.y;
	lightPos.z = vec.z;
}

void PhongShader::SetAmbientColor(float r, float g, float b, float a)
{
	Ka[0] = r;
	Ka[1] = g;
	Ka[2] = b;
	Ka[3] = a;
}

void PhongShader::SetDiffuseColor(float r, float g, float b, float a)
{
	Kd[0] = r;
	Kd[1] = g;
	Kd[2] = b;
	Kd[3] = a;
}

void PhongShader::SetSpecularColor(float r, float g, float b, float a)
{
	Ks[0] = r;
	Ks[1] = g;
	Ks[2] = b;
	Ks[3] = a;
}

void PhongShader::SetShininess(float shininess)
{
	Se = shininess;
}

void PhongShader::Begin()
{
	_shaderProgram->Begin();
}

void PhongShader::SetUniformsAndAttributes()
{
	if (_uvBufferID && _baseTexID)
	{
		glBindTexture(GL_TEXTURE_2D, _baseTexID);
	}

	GLuint programID = _shaderProgram->ProgramID();

	GLint projMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "projMat");
	GLint viewMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "viewMat");
	GLint normalMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "normalMat");
	GLint modelMatLoc = glGetUniformLocation(_shaderProgram->ProgramID(), "modelMat");

	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, Cam::GetInstance()->projMat.m);
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, Cam::GetInstance()->viewMat.m);
	glUniformMatrix3fv(normalMatLoc, 1, GL_FALSE, Cam::GetInstance()->GetNormalMat(_modelMat.m));
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, _modelMat.m);

	glUniform3f(glGetUniformLocation(programID, "lightPos"), 0.0, 0.0, 0.0);
	glUniform4f(glGetUniformLocation(programID, "ambient"), Ka[0], Ka[1], Ka[2], Ka[3]);
	glUniform4f(glGetUniformLocation(programID, "diffuse"), Kd[0], Kd[1], Kd[2], Kd[3]);
	glUniform4f(glGetUniformLocation(programID, "specular"), Ks[0], Ks[1], Ks[2], Ks[3]);
	glUniform1f(glGetUniformLocation(programID, "shininess"), Se);

	if (_uvBufferID && _baseTexID)
	{
		GLuint uvLoc = glGetAttribLocation(programID, "uv");
		glEnableVertexAttribArray(uvLoc);
		glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
		glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	if(_normalBufferID)
	{
		GLuint normalLoc = glGetAttribLocation(programID, "normal");
		glEnableVertexAttribArray(normalLoc);
		glBindBuffer(GL_ARRAY_BUFFER, _normalBufferID);
		glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
	}

	GLuint vertexLoc = glGetAttribLocation(programID, "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void PhongShader::End()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_uvBufferID && _baseTexID)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		GLuint uvLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "uv");
		glDisableVertexAttribArray(uvLoc);
	}

	if (_normalBufferID)
	{
		GLuint normalLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "normal");
		glDisableVertexAttribArray(normalLoc);
	}

	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glDisableVertexAttribArray(vertexLoc);

	_shaderProgram->End();
}
