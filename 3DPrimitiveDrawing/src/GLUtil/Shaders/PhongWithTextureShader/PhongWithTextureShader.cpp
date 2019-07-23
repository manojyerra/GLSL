#include "PhongWithTextureShader.h"
#include "ShadersManager.h"
#include "Cam.h"

PhongWithTextureShader::PhongWithTextureShader(int shaderType)
{
	_shaderType = shaderType;

	_perPixelShader = NULL;
	_perVertexShader = NULL;
	_shaderProgram = NULL;

	_vertexBufferID = 0;
	_normalBufferID = 0;
	_uvBufferID = 0;
	_baseTexID = 0;

	GLfloat Ka[] = { 0.05375,	0.05,	0.06625,	0.82 };
	GLfloat Kd[] = { 0.18275,	0.17,	0.22525,	0.82 };
	GLfloat Ks[] = { 0.332741,	0.328634,	0.346435,	0.82 };

	for (int i = 0; i < 4; i++)
	{
		_Ka[i] = Ka[i];
		_Kd[i] = Kd[i];
		_Ks[i] = Ks[i];
	}

	_Se = 38.4;

	_alpha = 1.0f;

	SetShaderType(_shaderType);
}

void PhongWithTextureShader::SetShaderType(int shaderType)
{
	_shaderType = shaderType;

	if (_shaderType == PER_PIXEL_SHADER)
	{
		if (_perPixelShader == NULL)
		{
			_perPixelShader = ShadersManager::GetInstance()->CreateShaderProgram(
				"shaders/PhongPerPixelWithTextureShader/PhongPerPixelWithTextureShader.vs",
				"shaders/PhongPerPixelWithTextureShader/PhongPerPixelWithTextureShader.fs");
		}

		_shaderProgram = _perPixelShader;
	}
	else if (_shaderType == PER_VERTEX_SHADER)
	{
		if (_perVertexShader == NULL)
		{
			_perVertexShader = ShadersManager::GetInstance()->CreateShaderProgram(
				"shaders/PhongPerVertexWithTextureShader/PhongPerVertexWithTextureShader.vs",
				"shaders/PhongPerVertexWithTextureShader/PhongPerVertexWithTextureShader.fs");
		}

		_shaderProgram = _perVertexShader;
	}
}

void PhongWithTextureShader::SetVertexBufferID(unsigned int bufferID)
{
	_vertexBufferID = bufferID;
}

void PhongWithTextureShader::SetNormalBufferID(unsigned int bufferID)
{
	_normalBufferID = bufferID;
}

void PhongWithTextureShader::SetUVBufferID(unsigned int bufferID)
{
	_uvBufferID = bufferID;
}

void PhongWithTextureShader::SetBaseTexID(unsigned int texID)
{
	_baseTexID = texID;
}

void PhongWithTextureShader::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void PhongWithTextureShader::SetLightPos(float x, float y, float z)
{
	lightPos.x = x;
	lightPos.y = y;
	lightPos.z = z;
}

void PhongWithTextureShader::SetAmbientColor(float r, float g, float b, float a)
{
	_Ka[0] = r;
	_Ka[1] = g;
	_Ka[2] = b;
	_Ka[3] = a;
}

void PhongWithTextureShader::SetDiffuseColor(float r, float g, float b, float a)
{
	_Kd[0] = r;
	_Kd[1] = g;
	_Kd[2] = b;
	_Kd[3] = a;
}

void PhongWithTextureShader::SetSpecularColor(float r, float g, float b, float a)
{
	_Ks[0] = r;
	_Ks[1] = g;
	_Ks[2] = b;
	_Ks[3] = a;
}

void PhongWithTextureShader::SetShininess(float shininess)
{
	_Se = shininess;
}

void PhongWithTextureShader::SetAlpha(float alpha)
{
	_alpha = alpha;
}

void PhongWithTextureShader::Begin()
{
	_shaderProgram->Begin();
}

void PhongWithTextureShader::SetUniformsAndAttributes()
{
	if (_uvBufferID && _baseTexID)
	{
		glBindTexture(GL_TEXTURE_2D, _baseTexID);
	}

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

	if (_uvBufferID && _baseTexID)
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

void PhongWithTextureShader::End()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_uvBufferID && _baseTexID)
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

PhongWithTextureShader::~PhongWithTextureShader()
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