#include "CubeGeometryShader.h"
#include "ShadersManager.h"
#include "Cam.h"

CubeGeometryShader::CubeGeometryShader()
{
	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram(
		"shaders/CubeGeometryShader/CubeGeometryShader.vs",
		"shaders/CubeGeometryShader/CubeGeometryShader.gs",
		"shaders/CubeGeometryShader/CubeGeometryShader.fs");

	_methodNum = 2;
	_cubeHalfLen = 0.0015f;
	_vertexBufferID = 0;
	_colorBufferID = 0;
	_alpha = 1.0f;
}

void CubeGeometryShader::SetCubeHalfLen(float cubeHalfLen)
{
	_cubeHalfLen = cubeHalfLen;
}

void CubeGeometryShader::SetAlpha(float alpha)
{
	_alpha = alpha;
}

void CubeGeometryShader::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void CubeGeometryShader::SetVertexBufferID(unsigned int bufferID)
{
	_vertexBufferID = bufferID;
}

void CubeGeometryShader::SetColorBufferID(unsigned int bufferID)
{
	_colorBufferID = bufferID;
}

void CubeGeometryShader::Begin()
{
	_shaderProgram->Begin();
}

void CubeGeometryShader::SetUniformsAndAttributes()
{
	Cam* cam = Cam::GetInstance();

	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam->GetMVP(_modelMat.m)));
	_shaderProgram->SetUniformMatrix3fv("normalMat", glm::value_ptr(cam->GetNormalMat(_modelMat.m)));
	_shaderProgram->SetUniformMatrix4fv("modelViewMat", glm::value_ptr(cam->GetModelViewMat(_modelMat.m)));
	_shaderProgram->SetUniform1f("hLen", _cubeHalfLen);
	_shaderProgram->SetUniform1i("methodNum", _methodNum);

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

void CubeGeometryShader::End()
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

CubeGeometryShader::~CubeGeometryShader()
{
	if(_shaderProgram)
	{
		ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}