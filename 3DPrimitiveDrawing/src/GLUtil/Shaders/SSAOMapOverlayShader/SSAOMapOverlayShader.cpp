#include "SSAOMapOverlayShader.h"
#include "ShaderProgramsManager.h"
#include "Cam2D.h"
#include "MathUtils.h"

SSAOMapOverlayShader::SSAOMapOverlayShader()
{
	_shaderProgram = NULL;
	_occlusionLevel = 0.5f;

	_shaderProgram = ShaderProgramsManager::GetInstance()->CreateShaderProgram("shaders/SSAO/SSAOMapOverlayShader/SSAOMapOverlayShader.vs",
		"shaders/SSAO/SSAOMapOverlayShader/SSAOMapOverlayShader.fs");
}

void SSAOMapOverlayShader::SetOcclusionLevel(float level)
{
	_occlusionLevel = MathUtils::MinMax(level, 0.0f, 1.0f);
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

	_shaderProgram->SetUniform1f("alpha", _alpha);
	_shaderProgram->SetUniform1f("occlusionLevel", _occlusionLevel);

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
		ShaderProgramsManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}