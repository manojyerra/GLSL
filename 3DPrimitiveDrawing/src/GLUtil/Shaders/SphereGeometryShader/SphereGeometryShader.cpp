#include "SphereGeometryShader.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"

SphereGeometryShader::SphereGeometryShader()
{
	_shaderProgram = ShaderProgramsManager::GetInstance()->CreateShaderProgram(
		"shaders/SphereGeometryShader/SphereGeometryShader.vs",
		"shaders/SphereGeometryShader/SphereGeometryShader.gs",
		"shaders/SphereGeometryShader/SphereGeometryShader.fs");

	_halfLen = 0.0015f;
}

void SphereGeometryShader::SetCubeHalfLen(float cubeHalfLen)
{
	_halfLen = cubeHalfLen;
}

void SphereGeometryShader::Begin()
{
	_shaderProgram->Begin();
}

void SphereGeometryShader::SetUniformsAndAttributes()
{
	Cam* cam = Cam::GetInstance();

	_shaderProgram->SetUniformMatrix4fv("projMat", glm::value_ptr(cam->GetProjMat()));
	_shaderProgram->SetUniformMatrix3fv("normalMat", glm::value_ptr(cam->GetNormalMat(_modelMat.m)));
	_shaderProgram->SetUniformMatrix4fv("modelViewMat", glm::value_ptr(cam->GetModelViewMat(_modelMat.m)));
	_shaderProgram->SetUniform1f("hLen", _halfLen);
	_shaderProgram->SetUniform1f("radiusPow2", _halfLen * _halfLen);

	GLuint colorLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
	glEnableVertexAttribArray(colorLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
	glVertexAttribPointer(colorLoc, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SphereGeometryShader::End()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_colorBufferID)
	{
		GLuint loc = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
		glDisableVertexAttribArray(loc);
	}

	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glDisableVertexAttribArray(vertexLoc);

	_shaderProgram->End();
}

SphereGeometryShader::~SphereGeometryShader()
{
	if(_shaderProgram)
	{
		ShaderProgramsManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}
