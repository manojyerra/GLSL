#include "BasicShader.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"

BasicShader::BasicShader()
{
	_color = glm::vec3(0.5f, 0.5f, 0.5f);

	_shaderProgram = ShaderProgramsManager::GetInstance()->CreateShaderProgram("shaders/BasicShader/BasicShader.vs",
		"shaders/BasicShader/BasicShader.fs");
}

void BasicShader::SetColor(glm::vec3 color)
{
	_color = color;
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
}

void BasicShader::End()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glDisableVertexAttribArray(vertexLoc);

	_shaderProgram->End();
}

BasicShader::~BasicShader()
{
	if (_shaderProgram)
	{
		ShaderProgramsManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}