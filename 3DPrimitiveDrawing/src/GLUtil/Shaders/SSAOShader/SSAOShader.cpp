#include "SSAOShader.h"
#include "ShadersManager.h"
#include "Cam.h"
#include "Cam2D.h"

SSAOShader::SSAOShader()
{
	_vertexBufferID = 0;
	_uvBufferID = 0;

	_gPositionTexID = 0;
	_gNormalTexID = 0;
	_noiseTexID = 0;

	_shaderProgram = nullptr;
	_alpha = 1.0f;

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram(
		"shaders/SSAOShader/SSAOShader.vs",
		"shaders/SSAOShader/SSAOShader.fs");
}

void SSAOShader::SetVertexBufferID(unsigned int bufferID)
{
	_vertexBufferID = bufferID;
}

void SSAOShader::SetUVBufferID(unsigned int bufferID)
{
	_uvBufferID = bufferID;
}

void SSAOShader::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void SSAOShader::SetAlpha(float alpha)
{
	_alpha = alpha;
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
	Cam2D* cam = Cam2D::GetInstance();
	//Cam* cam = Cam::GetInstance();

	float* m = _modelMat.m;

	GLuint programID = _shaderProgram->ProgramID();

	for (unsigned int i = 0; i < 64; ++i)
	{
		string uniName = "samples[" + std::to_string(i) + "]";
		glm::vec3 sample = _samples[i];
		_shaderProgram->SetUniform3f(uniName.c_str(), sample.x, sample.y, sample.z);
	}

	_shaderProgram->SetUniform1i("gPosition", 0);
	_shaderProgram->SetUniform1i("gNormal", 1);
	_shaderProgram->SetUniform1i("texNoise", 2);

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, _gPositionTexID);
	glActiveTexture(1);
	glBindTexture(GL_TEXTURE_2D, _gNormalTexID);
	glActiveTexture(2);
	glBindTexture(GL_TEXTURE_2D, _noiseTexID);

	_shaderProgram->SetUniformMatrix4fv("projection", glm::value_ptr(cam->GetProjMat()));
	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam->GetMVP(m)));

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
		ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}