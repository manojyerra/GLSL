#include "PBRShader.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"

PBRShader::PBRShader()
{
	CommonInit("shaders/PBR_ML/PBR_ML.vs",
		"shaders/PBR_ML/PBR_ML.fs");
}

PBRShader::PBRShader(string vertexShaderPath, string fragmentShaderPath)
{
	CommonInit(vertexShaderPath, fragmentShaderPath);
}

void PBRShader::CommonInit(string vertexShaderPath, string fragmentShaderPath)
{
	_shaderProgram = nullptr;

	/////////////////////////////////////////
	_mvpLoc = -1;
	_modelViewMatLoc = -1;
	_normalMatLoc = -1;

	_numLightsLoc = -1;
	_albedoLoc = -1;
	_metallicLoc = -1;
	_roughnessLoc = -1;
	_alphaLoc = -1;

	_normalLoc = -1;
	_vertexLoc = -1;

	_lightDirLocArr = new UIntArray(64);
	_lightColLocArr = new UIntArray(64);
	////////////////////////////////////////////

	LightInfo lightInfo1(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(23.47, 21.31, 20.79));
	LightInfo lightInfo2(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(23.47, 21.31, 20.79));
	LightInfo lightInfo3(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(23.47, 21.31, 20.79));
	LightInfo lightInfo4(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(23.47, 21.31, 20.79));
	LightInfo lightInfo5(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(23.47, 21.31, 20.79));

	_lightMap.clear();
	_lightMap.insert({ "0", lightInfo1 });
	_lightMap.insert({ "1", lightInfo2 });
	_lightMap.insert({ "2", lightInfo3 });
	_lightMap.insert({ "3", lightInfo4 });
	_lightMap.insert({ "4", lightInfo5 });

	_albedo = glm::vec3(0.08, 0.08, 0.08);
	_metallic = 1.0f;
	_roughness = 0.3f;

	_shaderProgram = ShaderProgramsManager::GetInstance()->CreateShaderProgram(vertexShaderPath, fragmentShaderPath);
	GenerateShaderLocations();
}

void PBRShader::GenerateShaderLocations()
{
	_shaderProgram->Begin();

	_mvpLoc = _shaderProgram->GetUniformLocation("mvp");
	_modelViewMatLoc = _shaderProgram->GetUniformLocation("modelViewMat");
	_normalMatLoc = _shaderProgram->GetUniformLocation("normalMat");

	_numLightsLoc = _shaderProgram->GetUniformLocation("numLights");
	_albedoLoc = _shaderProgram->GetUniformLocation("albedo");
	_metallicLoc = _shaderProgram->GetUniformLocation("metallic");
	_roughnessLoc = _shaderProgram->GetUniformLocation("roughness");
	_alphaLoc = _shaderProgram->GetUniformLocation("alpha");

	_normalLoc = _shaderProgram->GetAttribLocation("normal");
	_vertexLoc = _shaderProgram->GetAttribLocation("vertex");

	_lightDirLocArr->clear();
	_lightColLocArr->clear();

	std::map<string, LightInfo>::iterator iter;
	int count = 0;

	for (iter = _lightMap.begin(); iter != _lightMap.end(); ++iter)
	{
		string countStr = std::to_string(count);

		string dirUniName = "lightDir[" + countStr + "]";
		string colUniName = "lightCol[" + countStr + "]";

		_lightDirLocArr->push_back(_shaderProgram->GetUniformLocation(dirUniName.c_str()));
		_lightColLocArr->push_back(_shaderProgram->GetUniformLocation(colUniName.c_str()));

		count++;
	}

	_shaderProgram->End();
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

void PBRShader::SetMeterialProps(glm::vec3 albedo, float metallic, float roughtness)
{
	_albedo = albedo;
	_roughness = roughtness;
	_metallic = metallic;
}

glm::vec3 PBRShader::GetAlbedo()
{
	return _albedo;
}

float PBRShader::GetMetallic()
{
	return _metallic;
}

float PBRShader::GetRoughness()
{
	return _roughness;
}

std::map<string, LightInfo> PBRShader::GetLightMap()
{
	return _lightMap;
}

void PBRShader::AddLight(string lightName, LightInfo lightInfo)
{
	_lightMap.insert({ lightName, lightInfo });
	GenerateShaderLocations();
}

void PBRShader::RemoveLight(string lightName)
{
	_lightMap.erase(lightName);
	GenerateShaderLocations();
}

void PBRShader::Begin()
{
	_shaderProgram->Begin();
}

void PBRShader::SetUniformsAndAttributes()
{
	Cam* cam = Cam::GetInstance();
	float* m = _modelMat.m;

	_shaderProgram->SetUniformMatrix4fv(_mvpLoc, glm::value_ptr(cam->GetMVP(m)));
	_shaderProgram->SetUniformMatrix4fv(_modelViewMatLoc, glm::value_ptr(cam->GetModelViewMat(m)));
	_shaderProgram->SetUniformMatrix3fv(_normalMatLoc, glm::value_ptr(cam->GetNormalMat(m)));

	std::map<std::string, LightInfo>::iterator iter;
	int count = 0;

	for (iter = _lightMap.begin(); iter != _lightMap.end(); ++iter)
	{
		_shaderProgram->SetUniform3f(_lightDirLocArr->get(count), iter->second.dir);
		_shaderProgram->SetUniform3f(_lightColLocArr->get(count), iter->second.color);

		count++;
	}

	_shaderProgram->SetUniform1i(_numLightsLoc, (int)_lightMap.size());

	_shaderProgram->SetUniform3f(_albedoLoc, _albedo.r, _albedo.g, _albedo.b);
	_shaderProgram->SetUniform1f(_metallicLoc, _metallic);
	_shaderProgram->SetUniform1f(_roughnessLoc, _roughness);
	_shaderProgram->SetUniform1f(_alphaLoc, _alpha);

	if (_normalBufferID)
	{
		glEnableVertexAttribArray(_normalLoc);
		glBindBuffer(GL_ARRAY_BUFFER, _normalBufferID);
		glVertexAttribPointer(_normalLoc, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
	}

	glEnableVertexAttribArray(_vertexLoc);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
	glVertexAttribPointer(_vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

//void PBRShader::SetUniformsAndAttributes()
//{
//	Cam* cam = Cam::GetInstance();
//	float* m = _modelMat.m;
//
//	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam->GetMVP(m)));
//	_shaderProgram->SetUniformMatrix4fv("modelViewMat", glm::value_ptr(cam->GetModelViewMat(m)));
//	_shaderProgram->SetUniformMatrix3fv("normalMat", glm::value_ptr(cam->GetNormalMat(m)));
//
//	std::map<string, LightInfo>::iterator iter;
//	int count = 0;
//
//	for (iter = _lightMap.begin(); iter != _lightMap.end(); ++iter)
//	{
//		string countStr = std::to_string(count);
//
//		string dirUniName = "lightDir[" + countStr + "]";
//		string colUniName = "lightCol[" + countStr + "]";
//
//		glm::vec3 dir(iter->second.dir);
//		glm::vec3 col(iter->second.color);
//
//		_shaderProgram->SetUniform3f(dirUniName.c_str(), dir.x, dir.y, dir.z);
//		_shaderProgram->SetUniform3f(colUniName.c_str(), col.x, col.y, col.z);
//
//		count++;
//	}
//
//	_shaderProgram->SetUniform1i("numLights", (int)_lightMap.size());
//
//	_shaderProgram->SetUniform3f("albedo", _albedo.r, _albedo.g, _albedo.b);
//	_shaderProgram->SetUniform1f("metallic", _metallic);
//	_shaderProgram->SetUniform1f("roughness", _roughness);
//	_shaderProgram->SetUniform1f("alpha", _alpha);
//
//	if (_normalBufferID)
//	{
//		GLuint normalLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "normal");
//		glEnableVertexAttribArray(normalLoc);
//		glBindBuffer(GL_ARRAY_BUFFER, _normalBufferID);
//		glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
//	}
//
//	GLuint vertexLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "vertex");
//	glEnableVertexAttribArray(vertexLoc);
//	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferID);
//	glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//}

void PBRShader::End()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_normalBufferID)
	{
		glDisableVertexAttribArray(_normalLoc);
	}

	glDisableVertexAttribArray(_vertexLoc);

	_shaderProgram->End();
}

PBRShader::~PBRShader()
{
	if (_shaderProgram)
	{
		ShaderProgramsManager::GetInstance()->DeleteShaderProgram(_shaderProgram);
		_shaderProgram = NULL;
	}
}