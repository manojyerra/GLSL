#include "SSAOBlurShader.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"

SSAOBlurShader::SSAOBlurShader()
{
	_alpha = 1.0f;
	_ssaoInputTexID = 0;

	_shaderProgram = ShaderProgramsManager::GetInstance()->CreateShaderProgram(
		"shaders/SSAO/SSAOBlurShader/SSAOBlurShader.vs",
		"shaders/SSAO/SSAOBlurShader/SSAOBlurShader.fs");
}

void SSAOBlurShader::SetSSAOInputTexID(unsigned int texID)
{
	_ssaoInputTexID = texID;
}

void SSAOBlurShader::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void SSAOBlurShader::SetAlpha(float alpha)
{
	_alpha = alpha;
}

void SSAOBlurShader::Begin()
{
	_shaderProgram->Begin();
}

void SSAOBlurShader::SetUniformsAndAttributes()
{
	GLuint programID = _shaderProgram->ProgramID();

	_shaderProgram->SetUniform1i("ssaoInput", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _ssaoInputTexID);

	if (_uvBufferID)
	{
		GLuint uvLoc = glGetAttribLocation(programID, "uv");
		glEnableVertexAttribArray(uvLoc);
		glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
		glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	GLuint vertexLoc = glGetAttribLocation(programID, "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void SSAOBlurShader::End()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_uvBufferID)
	{
		GLuint uvLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "uv");
		glDisableVertexAttribArray(uvLoc);
	}

	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glDisableVertexAttribArray(vertexLoc);

	_shaderProgram->End();
}

SSAOBlurShader::~SSAOBlurShader()
{
	if (_shaderProgram)
	{
		ShaderProgramsManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}