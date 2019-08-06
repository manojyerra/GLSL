#include "ColorShader.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"
#include "Cam2D.h"

ColorShader::ColorShader()
{
	_shaderProgram = NULL;

	_vertexBufferID = 0;
	_colorBufferID = 0;

	_alpha = 1.0f;
	_use2DCam = false;

	_shaderProgram = ShaderProgramsManager::GetInstance()->CreateShaderProgram("shaders/ColorShader/ColorShader.vs",
		"shaders/ColorShader/ColorShader.fs");
}

void ColorShader::Set2DCamera(bool enable)
{
	_use2DCam = enable;
}

void ColorShader::SetVertexBufferID(unsigned int bufferID)
{
	_vertexBufferID = bufferID;
}

void ColorShader::SetColorBufferID(unsigned int bufferID)
{
	_colorBufferID = bufferID;
}

void ColorShader::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void ColorShader::SetAlpha(float alpha)
{
	_alpha = alpha;
}

void ColorShader::Begin()
{
	_shaderProgram->Begin();
}

void ColorShader::SetUniformsAndAttributes()
{
	GLuint programID = _shaderProgram->ProgramID();
	float* m = _modelMat.m;

	if (_use2DCam)
	{
		Cam2D* cam2D = Cam2D::GetInstance();
		_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam2D->GetMVP(m)));
	}
	else
	{
		Cam* cam3D = Cam::GetInstance();
		_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam3D->GetMVP(m)));
	}

	_shaderProgram->SetUniform1f("alpha", _alpha);

	if (_colorBufferID)
	{
		GLuint loc = glGetAttribLocation(programID, "color");
		glEnableVertexAttribArray(loc);
		glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
		glVertexAttribPointer(loc, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, (void*)0);
	}

	GLuint vertexLoc = glGetAttribLocation(programID, "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void ColorShader::End()
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

ColorShader::~ColorShader()
{
	if (_shaderProgram)
	{
		ShaderProgramsManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}