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
	_uvBufferID = 0;
	_baseTexID = 0;

	Ka[0] = Ka[1] = Ka[2] = 0.05f;
	Kd[0] = Kd[1] = Kd[2] = 0.20f;
	Ks[0] = Ks[1] = Ks[2] = 0.32f;

	Ka[3] = 1.0f;
	Kd[3] = 1.0f;
	Ks[3] = 1.0f;
	
	Se = 38.4f;

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
				"shaders/Phong/Phong.vs", 
				"shaders/Phong/Phong.fs");
		}

		_shaderProgram = _perPixelShader;
	}
	else if (_shaderType == PER_VERTEX_SHADER)
	{
		if (_perVertexShader == NULL)
		{
			_perVertexShader = ShadersManager::GetInstance()->CreateShaderProgram(
				"shaders/PhongPerVertex/PhongPerVertex.vs", 
				"shaders/PhongPerVertex/PhongPerVertex.fs");
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

void PhongShader::SetUVBufferID(unsigned int bufferID)
{
	_uvBufferID = bufferID;
}

void PhongShader::SetBaseTexID(unsigned int texID)
{
	_baseTexID = texID;
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
	Ka[0] = r;
	Ka[1] = g;
	Ka[2] = b;
	Ka[3] = a;
}

void PhongShader::SetDiffuseColor(float r, float g, float b, float a)
{
	Kd[0] = r;
	Kd[1] = g;
	Kd[2] = b;
	Kd[3] = a;
}

void PhongShader::SetSpecularColor(float r, float g, float b, float a)
{
	Ks[0] = r;
	Ks[1] = g;
	Ks[2] = b;
	Ks[3] = a;
}

void PhongShader::SetShininess(float shininess)
{
	Se = shininess;
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
	_shaderProgram->SetUniform4f("ambient", Ka[0], Ka[1], Ka[2], Ka[3]);
	_shaderProgram->SetUniform4f("diffuse", Kd[0], Kd[1], Kd[2], Kd[3]);
	_shaderProgram->SetUniform4f("specular", Ks[0], Ks[1], Ks[2], Ks[3]);
	_shaderProgram->SetUniform1f("shininess", Se);

	if (_uvBufferID && _baseTexID)
	{
		GLuint uvLoc = glGetAttribLocation(programID, "uv");
		glEnableVertexAttribArray(uvLoc);
		glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
		glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	if(_normalBufferID)
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