#include "SSAOMapOverlayShader.h"
#include "ShadersManager.h"
#include "Cam2D.h"

SSAOMapOverlayShader::SSAOMapOverlayShader()
{
	_shaderProgram = NULL;

	_vertexBufferID = 0;
	_uvBufferID = 0;
	_textureID = 0;

	_alpha = 1.0f;

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram("shaders/SSAO/SSAOMapOverlayShader/SSAOMapOverlayShader.vs",
		"shaders/SSAO/SSAOMapOverlayShader/SSAOMapOverlayShader.fs");
}

void SSAOMapOverlayShader::SetVertexBufferID(unsigned int bufferID)
{
	_vertexBufferID = bufferID;
}

void SSAOMapOverlayShader::SetUVBufferID(unsigned int bufferID)
{
	_uvBufferID = bufferID;
}

void SSAOMapOverlayShader::SetTextureID(unsigned int textureID)
{
	_textureID = textureID;
}

void SSAOMapOverlayShader::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void SSAOMapOverlayShader::SetAlpha(float alpha)
{
	_alpha = alpha;
}

void SSAOMapOverlayShader::Set2DCamera(bool enable)
{
	_use2DCam = enable;
}

void SSAOMapOverlayShader::Begin()
{
	_shaderProgram->Begin();

	_shaderProgram->SetUniform1i("textureID", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID);
}

void SSAOMapOverlayShader::SetUniformsAndAttributes()
{
	GLuint programID = _shaderProgram->ProgramID();

	float* m = _modelMat.m;

	Cam2D* cam2D = Cam2D::GetInstance();
	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam2D->GetMVP(m)));
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

void SSAOMapOverlayShader::End()
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

SSAOMapOverlayShader::~SSAOMapOverlayShader()
{
	if (_shaderProgram)
	{
		ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}