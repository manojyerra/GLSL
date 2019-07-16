#include "PBRShader.h"
#include "ShadersManager.h"
#include "Cam.h"

PBRShader::PBRShader()
{
	_shaderProgram = NULL;

	_vertexBufferID = 0;
	_normalBufferID = 0;

	_lightDir = glm::vec3(0.0f, -1.0f, 0.0f);
	_lightColor = glm::vec3(23.47, 21.31, 20.79);

	_albedo = glm::vec3(0.95, 0.93, 0.88);
	_metallic = 0.9f;
	_roughness = 0.3f;
	_alpha = 1.0f;

	_shaderProgram = ShadersManager::GetInstance()->CreateShaderProgram("shaders/PBR_ML/PBR_ML.vs",
																		"shaders/PBR_ML/PBR_ML.fs");
}

void PBRShader::SetVertexBufferID(unsigned int bufferID)
{
	_vertexBufferID = bufferID;
}

void PBRShader::SetNormalBufferID(unsigned int bufferID)
{
	_normalBufferID = bufferID;
}

void PBRShader::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void PBRShader::SetAlpha(float alpha)
{
	_alpha = alpha;
}

void PBRShader::SetLightDirection(glm::vec3& lightDir)
{
	_lightDir = lightDir;
}

void PBRShader::SetLightColor(glm::vec3& lightColor)
{
	_lightColor = lightColor;
}

void PBRShader::SetAlbedo(glm::vec3& albedo)
{
	_albedo = albedo;
}

void PBRShader::SetMetallic(float metallic)
{
	_metallic = metallic;
}

void PBRShader::SetRoughness(float roughness)
{
	_roughness = roughness;
}

void PBRShader::Begin()
{
	_shaderProgram->Begin();
}

void PBRShader::SetUniformsAndAttributes()
{
	Cam* cam = Cam::GetInstance();
	float* m = _modelMat.m;

	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam->GetMVP(m)));
	_shaderProgram->SetUniformMatrix4fv("modelViewMat", glm::value_ptr(cam->GetModelViewMat(m)));
	_shaderProgram->SetUniformMatrix3fv("normalMat", glm::value_ptr(cam->GetNormalMat(m)));
	//_shaderProgram->SetUniformMatrix4fv("camMat", cam->viewMat.m);

	//glm::vec4 lightDir = glm::vec4(0, -1, 0, 0);
	//lightDir = glm::normalize(viewMatrix * lightDir);
	_shaderProgram->SetUniform3f("direction", _lightDir.x, _lightDir.y, _lightDir.z);
	_shaderProgram->SetUniform3f("color", _lightColor.r, _lightColor.g, _lightColor.b);
	_shaderProgram->SetUniform3f("albedo", _albedo.r, _albedo.g, _albedo.b);
	_shaderProgram->SetUniform1f("metallic", _metallic);
	_shaderProgram->SetUniform1f("roughness", _roughness);
	_shaderProgram->SetUniform1f("alpha", _alpha);

	if(_normalBufferID)
	{
		GLuint normalLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "normal");
		glEnableVertexAttribArray(normalLoc);
		glBindBuffer(GL_ARRAY_BUFFER, _normalBufferID);
		glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
	}

	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
	glEnableVertexAttribArray(vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void PBRShader::End()
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

PBRShader::~PBRShader()
{
	if (_shaderProgram)
	{
		ShadersManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}