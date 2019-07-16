#include "ColorShader.h"
#include "ShadersManager.h"
#include "Cam.h"

ColorShader::ColorShader()
{
	_shaderProgram = NULL;

	_vertexBufferID = 0;
	_colorBufferID = 0;

	_alpha = 1.0f;

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram( "shaders/ColorArray/ColorArray.vs",
																			"shaders/ColorArray/ColorArray.fs");
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
	Cam* cam = Cam::GetInstance();

	GLuint programID = _shaderProgram->ProgramID();

	float* m = _modelMat.m;

	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam->GetMVP(m)));
	_shaderProgram->SetUniform1f("alpha", _alpha);

	if(_colorBufferID)
	{
		GLuint loc = glGetAttribLocation(programID, "color");
		glEnableVertexAttribArray(loc);
		glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
		glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
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
		ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}
