#include "BasicShader.h"
#include "ShadersManager.h"
#include "Cam.h"

BasicShader::BasicShader()
{
	_vertexBufferID = 0;

	_alpha = 1.0f;
	_color = glm::vec3(0.5f, 0.5f, 0.5f);

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram("shaders/SingleColor/SingleColor.vs",
																		"shaders/SingleColor/SingleColor.fs");
}

void BasicShader::SetVertexBufferID(unsigned int bufferID)
{
	_vertexBufferID = bufferID;
}

void BasicShader::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void BasicShader::SetAlpha(float alpha)
{
	_alpha = alpha;
}

void BasicShader::Begin()
{
	_shaderProgram->Begin();
}

void BasicShader::SetUniformsAndAttributes()
{
	Cam* cam = Cam::GetInstance();

	GLuint programID = _shaderProgram->ProgramID();

	float* m = _modelMat.m;

	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam->GetMVP(m)));
	_shaderProgram->SetUniform3f("color", _color.r, _color.g, _color.b);
	_shaderProgram->SetUniform1f("alpha", _alpha);

	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BasicShader::End()
{
	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glDisableVertexAttribArray(vertexLoc);

	_shaderProgram->End();
}

BasicShader::~BasicShader()
{
	if (_shaderProgram)
	{
		ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}