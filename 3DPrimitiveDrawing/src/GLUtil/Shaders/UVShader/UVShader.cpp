#include "UVShader.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"
#include "Cam2D.h"

UVShader::UVShader()
{
	_shaderProgram = NULL;

	_vertexBufferID = 0;
	_uvBufferID = 0;
	_textureID = 0;

	_alpha = 1.0f;
	_use2DCam = false;

	_shaderProgram = ShaderProgramsManager::GetInstance()->CreateShaderProgram("shaders/UVShader/UVShader.vs",
		"shaders/UVShader/UVShader.fs");
}

void UVShader::SetVertexBufferID(unsigned int bufferID)
{
	_vertexBufferID = bufferID;
}

void UVShader::SetUVBufferID(unsigned int bufferID)
{
	_uvBufferID = bufferID;
}

void UVShader::SetTextureID(unsigned int textureID)
{
	_textureID = textureID;
}

void UVShader::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void UVShader::SetAlpha(float alpha)
{
	_alpha = alpha;
}

void UVShader::Set2DCamera(bool enable)
{
	_use2DCam = enable;
}

void UVShader::Begin()
{
	_shaderProgram->Begin();

	_shaderProgram->SetUniform1i("textureID", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);
}

void UVShader::SetUniformsAndAttributes()
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

	if (_uvBufferID)
	{
		GLuint loc = glGetAttribLocation(programID, "uv");
		glEnableVertexAttribArray(loc);
		glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
		glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	GLuint vertexLoc = glGetAttribLocation(programID, "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void UVShader::End()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_uvBufferID)
	{
		GLuint loc = glGetAttribLocation(_shaderProgram->ProgramID(), "uv");
		glDisableVertexAttribArray(loc);
	}

	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glDisableVertexAttribArray(vertexLoc);

	_shaderProgram->End();
}

UVShader::~UVShader()
{
	if (_shaderProgram)
	{
		ShaderProgramsManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}