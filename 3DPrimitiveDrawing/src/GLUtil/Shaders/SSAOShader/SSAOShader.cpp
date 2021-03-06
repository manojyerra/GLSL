#include "SSAOShader.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"

SSAOShader::SSAOShader()
{
	_gPositionTexID = 0;
	_gNormalTexID = 0;
	_noiseTexID = 0;
	_screenSize = glm::vec2(1280.0f, 720.0f);
	_shaderProgram = nullptr;

	_shaderProgram = ShaderProgramsManager::GetInstance()->CreateShaderProgram(
		"shaders/SSAO/SSAOCalcShader/SSAOCalcShader.vs",
		"shaders/SSAO/SSAOCalcShader/SSAOCalcShader.fs");
}

void SSAOShader::SetGPositionTexID(unsigned int texID)
{
	_gPositionTexID = texID;
}

void SSAOShader::SetGNormalTexID(unsigned int texID)
{
	_gNormalTexID = texID;
}

void SSAOShader::SetNoiseTexID(unsigned int texID)
{
	_noiseTexID = texID;
}

void SSAOShader::SetScreenSize(float sw, float sh)
{
	_screenSize = glm::vec2(sw, sh);
}

void SSAOShader::SetSamples(std::vector<glm::vec3> samples)
{
	_samples = samples;
}

void SSAOShader::Begin()
{
	_shaderProgram->Begin();
}

void SSAOShader::SetUniformsAndAttributes()
{
	GLuint programID = _shaderProgram->ProgramID();

	for (unsigned int i = 0; i < _samples.size(); ++i)
	{
		string uniName = "samples[" + std::to_string(i) + "]";
		glm::vec3 sample = _samples[i];
		_shaderProgram->SetUniform3f(uniName.c_str(), sample.x, sample.y, sample.z);
	}
	_shaderProgram->SetUniformMatrix4fv("projection", glm::value_ptr(Cam::GetInstance()->GetProjMat()));

	_shaderProgram->SetUniform1i("gPosition", 0);
	_shaderProgram->SetUniform1i("gNormal", 1);
	_shaderProgram->SetUniform1i("texNoise", 2);
	_shaderProgram->SetUniform2f("screenSize", _screenSize);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _gPositionTexID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _gNormalTexID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _noiseTexID);

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

void SSAOShader::End()
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

SSAOShader::~SSAOShader()
{
	if (_shaderProgram)
	{
		ShaderProgramsManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}