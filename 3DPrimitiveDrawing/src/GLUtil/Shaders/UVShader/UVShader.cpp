#include "UVShader.h"
#include "ShadersManager.h"
#include "Cam.h"

UVShader::UVShader()
{
	_shaderProgram = NULL;

	_vertexBufferID = 0;
	_uvBufferID = 0;
	_textureID = 0;

	_alpha = 1.0f;

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram( "shaders/UVShader/UVShader.vs",
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

void UVShader::Begin()
{
	_shaderProgram->Begin();
	glBindTexture(GL_TEXTURE_2D, _textureID);
}

void UVShader::SetUniformsAndAttributes()
{
	Cam* cam = Cam::GetInstance();

	GLuint programID = _shaderProgram->ProgramID();

	float* m = _modelMat.m;

	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam->GetMVP(m)));
	_shaderProgram->SetUniform1f("alpha", _alpha);

	if(_uvBufferID)
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
		ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}
