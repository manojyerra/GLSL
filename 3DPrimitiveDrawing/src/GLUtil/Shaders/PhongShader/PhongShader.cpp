#include "PhongShader.h"
#include "ShadersManager.h"
#include "Cam.h"

PhongShader::PhongShader(int shaderType)
{
	_shaderType = shaderType;

	_perPixelShader = NULL;
	_perVertexShader = NULL;
	_shaderProgram = NULL;

	_vertexBufferID = 0;
	_normalBufferID = 0;

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

	SetShaderType(_shaderType);
}

void PhongShader::SetShaderType(int shaderType)
{
	_shaderType = shaderType;

	if (_shaderType == PER_PIXEL_SHADER)
	{
		if (_perPixelShader == NULL)
		{
			_perPixelShader = ShadersManager::GetInstance()->CreateShaderProgram(
				"shaders/PhongPerPixelShader/PhongPerPixelShader.vs",
				"shaders/PhongPerPixelShader/PhongPerPixelShader.fs");
		}

		_shaderProgram = _perPixelShader;
	}
	else if (_shaderType == PER_VERTEX_SHADER)
	{
		if (_perVertexShader == NULL)
		{
			_perVertexShader = ShadersManager::GetInstance()->CreateShaderProgram(
				"shaders/PhongPerVertexShader/PhongPerVertexShader.vs",
				"shaders/PhongPerVertexShader/PhongPerVertexShader.fs");
		}

		_shaderProgram = _perVertexShader;
	}
}

void PhongShader::SetVertexBufferID(unsigned int bufferID)
{
	_vertexBufferID = bufferID;
}

void PhongShader::SetNormalBufferID(unsigned int bufferID)
{
	_normalBufferID = bufferID;
}

void PhongShader::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void PhongShader::SetLightPos(float x, float y, float z)
{
	lightPos.x = x;
	lightPos.y = y;
	lightPos.z = z;
}

void PhongShader::SetAmbientColor(float r, float g, float b, float a)
{
	_Ka[0] = r;
	_Ka[1] = g;
	_Ka[2] = b;
	_Ka[3] = a;
}

void PhongShader::SetDiffuseColor(float r, float g, float b, float a)
{
	_Kd[0] = r;
	_Kd[1] = g;
	_Kd[2] = b;
	_Kd[3] = a;
}

void PhongShader::SetSpecularColor(float r, float g, float b, float a)
{
	_Ks[0] = r;
	_Ks[1] = g;
	_Ks[2] = b;
	_Ks[3] = a;
}

void PhongShader::SetShininess(float shininess)
{
	_Se = shininess;
}

void PhongShader::SetAlpha(float alpha)
{
	_alpha = alpha;
}

void PhongShader::Begin()
{
	_shaderProgram->Begin();
}

void PhongShader::SetUniformsAndAttributes()
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
	_shaderProgram->SetUniform4f("diffuse", _Kd[0], _Kd[1], _Kd[2], _Kd[3]);
	_shaderProgram->SetUniform4f("specular", _Ks[0], _Ks[1], _Ks[2], _Ks[3]);
	_shaderProgram->SetUniform1f("shininess", _Se);

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

void PhongShader::End()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_normalBufferID)
	{
		GLuint normalLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "normal");
		glDisableVertexAttribArray(normalLoc);
	}

	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glDisableVertexAttribArray(vertexLoc);

	_shaderProgram->End();
}

PhongShader::~PhongShader()
{
	if (_perPixelShader)
	{
		ShadersManager::GetInstance()->DeleteShaderProgram(_perPixelShader);
		_perPixelShader = NULL;
	}

	if (_perVertexShader)
	{
		ShadersManager::GetInstance()->DeleteShaderProgram(_perVertexShader);
		_perVertexShader = NULL;
	}

	_shaderProgram = NULL;
}