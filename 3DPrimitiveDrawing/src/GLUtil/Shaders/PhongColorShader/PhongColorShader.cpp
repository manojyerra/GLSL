#include "PhongColorShader.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"

PhongColorShader::PhongColorShader()
{
	_shaderProgram = NULL;

	GLfloat Ka[] = { 0.05375f,	0.05f,	0.06625f,	1.0f };
	GLfloat Kd[] = { 0.18275f,	0.17f,	0.22525f,	1.0f };
	GLfloat Ks[] = { 0.33274f,	0.32f,	0.34643f,	1.0f };

	for (int i = 0; i < 4; i++)
	{
		_Ka[i] = Ka[i];
		_Kd[i] = Kd[i];
		_Ks[i] = Ks[i];
	}

	_Se = 38.4f;
	_alpha = 1.0f;

	_shaderProgram = ShaderProgramsManager::GetInstance()->CreateShaderProgram(
		"shaders/PhongColorShader/PhongColorShader.vs",
		"shaders/PhongColorShader/PhongColorShader.fs");
}

void PhongColorShader::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void PhongColorShader::SetLightPos(float x, float y, float z)
{
	lightPos.x = x;
	lightPos.y = y;
	lightPos.z = z;
}

void PhongColorShader::SetAmbientColor(float r, float g, float b, float a)
{
	_Ka[0] = r;
	_Ka[1] = g;
	_Ka[2] = b;
	_Ka[3] = a;
}

void PhongColorShader::SetSpecularColor(float r, float g, float b, float a)
{
	_Ks[0] = r;
	_Ks[1] = g;
	_Ks[2] = b;
	_Ks[3] = a;
}

void PhongColorShader::SetShininess(float shininess)
{
	_Se = shininess;
}

void PhongColorShader::SetAlpha(float alpha)
{
	_alpha = alpha;
}

void PhongColorShader::Begin()
{
	_shaderProgram->Begin();
}

void PhongColorShader::SetUniformsAndAttributes()
{
	Cam* cam = Cam::GetInstance();

	GLuint programID = _shaderProgram->ProgramID();

	float* m = _modelMat.m;

	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam->GetMVP(m)));
	_shaderProgram->SetUniformMatrix4fv("modelView", glm::value_ptr(cam->GetModelViewMat(m)));
	_shaderProgram->SetUniformMatrix3fv("normalMat", glm::value_ptr(cam->GetNormalMat(m)));
	_shaderProgram->SetUniform1f("alpha", _alpha);

	_shaderProgram->SetUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
	_shaderProgram->SetUniform4f("ambient", _Ka[0], _Ka[1], _Ka[2], _Ka[3]);
	_shaderProgram->SetUniform4f("specular", _Ks[0], _Ks[1], _Ks[2], _Ks[3]);
	_shaderProgram->SetUniform1f("shininess", _Se);

	if (_colorBufferID)
	{
		GLuint loc = glGetAttribLocation(programID, "color");
		glEnableVertexAttribArray(loc);
		glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
		glVertexAttribPointer(loc, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, (void*)0);
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

void PhongColorShader::End()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_colorBufferID)
	{
		GLuint loc = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
		glDisableVertexAttribArray(loc);
	}

	if (_normalBufferID)
	{
		GLuint loc = glGetAttribLocation(_shaderProgram->ProgramID(), "normal");
		glDisableVertexAttribArray(loc);
	}

	GLuint loc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glDisableVertexAttribArray(loc);

	_shaderProgram->End();
}

PhongColorShader::~PhongColorShader()
{
	if (_shaderProgram)
	{
		ShaderProgramsManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}