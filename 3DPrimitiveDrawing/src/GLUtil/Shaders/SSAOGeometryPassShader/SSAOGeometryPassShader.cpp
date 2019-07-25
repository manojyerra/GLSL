#include "SSAOGeometryPassShader.h"
#include "ShadersManager.h"
#include "Cam.h"

SSAOGeometryPassShader::SSAOGeometryPassShader()
{
	_shaderProgram = nullptr;

	_vertexBufferID = 0;
	_normalBufferID = 0;
	_uvBufferID = 0;
	_alpha = 1.0f;

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram(
		"shaders/SSAOGeometryPassShader/SSAOGeometryPassShader.vs",
		"shaders/SSAOGeometryPassShader/SSAOGeometryPassShader.fs");
}

void SSAOGeometryPassShader::SetVertexBufferID(unsigned int bufferID)
{
	_vertexBufferID = bufferID;
}

void SSAOGeometryPassShader::SetNormalBufferID(unsigned int bufferID)
{
	_normalBufferID = bufferID;
}

void SSAOGeometryPassShader::SetUVBufferID(unsigned int bufferID)
{
	_uvBufferID = bufferID;
}

void SSAOGeometryPassShader::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void SSAOGeometryPassShader::SetAlpha(float alpha)
{
	_alpha = alpha;
}

void SSAOGeometryPassShader::Begin()
{
	_shaderProgram->Begin();
}

void SSAOGeometryPassShader::SetUniformsAndAttributes()
{
	Cam* cam = Cam::GetInstance();

	GLuint programID = _shaderProgram->ProgramID();

	float* m = _modelMat.m;

	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam->GetMVP(m)));
	_shaderProgram->SetUniformMatrix4fv("modelView", glm::value_ptr(cam->GetModelViewMat(m)));
	_shaderProgram->SetUniformMatrix3fv("normalMat", glm::value_ptr(cam->GetNormalMat(m)));
	//_shaderProgram->SetUniform1f("alpha", _alpha);

	if (_uvBufferID)
	{
		GLuint uvLoc = glGetAttribLocation(programID, "uv");
		glEnableVertexAttribArray(uvLoc);
		glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
		glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	if (_normalBufferID)
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

void SSAOGeometryPassShader::End()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_uvBufferID)
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

SSAOGeometryPassShader::~SSAOGeometryPassShader()
{
	if (_shaderProgram)
	{
		ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}