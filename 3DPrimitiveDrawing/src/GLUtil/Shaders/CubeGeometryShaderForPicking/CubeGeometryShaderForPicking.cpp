#include "CubeGeometryShaderForPicking.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"

CubeGeometryShaderForPicking::CubeGeometryShaderForPicking()
{
	_shaderProgram = ShaderProgramsManager::GetInstance()->CreateShaderProgram(
		"shaders/CubeGeometryShaderForPicking/CubeGeometryShaderForPicking.vs",
		"shaders/CubeGeometryShaderForPicking/CubeGeometryShaderForPicking.gs",
		"shaders/CubeGeometryShaderForPicking/CubeGeometryShaderForPicking.fs");

	_halfLen = 0.00375f/2.0f;
}

void CubeGeometryShaderForPicking::SetCubeHalfLen(float cubeHalfLen)
{
	_halfLen = cubeHalfLen;
}

void CubeGeometryShaderForPicking::Begin()
{
	_shaderProgram->Begin();
}

void CubeGeometryShaderForPicking::SetUniformsAndAttributes()
{
	Cam* cam = Cam::GetInstance();

	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam->GetMVP(_modelMat.m)));
	_shaderProgram->SetUniformMatrix4fv("modelViewMat", glm::value_ptr(cam->GetModelViewMat(_modelMat.m)));
	_shaderProgram->SetUniform1f("hLen", _halfLen);
	_shaderProgram->SetUniform1f("alpha", _alpha);

	if(_colorBufferID)
	{
		GLuint colorLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
		glEnableVertexAttribArray(colorLoc);
		glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
		glVertexAttribPointer(colorLoc, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CubeGeometryShaderForPicking::End()
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

CubeGeometryShaderForPicking::~CubeGeometryShaderForPicking()
{
	if(_shaderProgram)
	{
		ShaderProgramsManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}
