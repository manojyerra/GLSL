#include "PBRCubeGeometryShader.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"

PBRCubeGeometryShader::PBRCubeGeometryShader()
{
	_metallic = 1.0f;
	_roughness = 0.23f;
	_halfLen = 0.00375f / 2.0f;

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

	_shaderProgram = ShaderProgramsManager::GetInstance()->CreateShaderProgram("shaders/PBRCubeGeometryShader/PBRCubeGeometryShader.vs",
																				"shaders/PBRCubeGeometryShader/PBRCubeGeometryShader.gs",
																				"shaders/PBRCubeGeometryShader/PBRCubeGeometryShader.fs");
}

void PBRCubeGeometryShader::SetCubeHalfLen(float cubeHalfLen)
{
	_halfLen = cubeHalfLen;
}

void PBRCubeGeometryShader::Begin()
{
	_shaderProgram->Begin();
}

void PBRCubeGeometryShader::SetUniformsAndAttributes()
{
	Cam* cam = Cam::GetInstance();
	float* m = _modelMat.m;

	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam->GetMVP(m)));
	_shaderProgram->SetUniformMatrix4fv("modelViewMat", glm::value_ptr(cam->GetModelViewMat(m)));
	_shaderProgram->SetUniformMatrix3fv("normalMat", glm::value_ptr(cam->GetNormalMat(m)));
	_shaderProgram->SetUniform1f("hLen", _halfLen);
	_shaderProgram->SetUniform1f("alpha", _alpha);

	std::map<string, LightInfo>::iterator iter;
	int count = 0;

	for (iter = _lightMap.begin(); iter != _lightMap.end(); ++iter)
	{
		string countStr = std::to_string(count);

		string dirUniName = "lightDir[" + countStr + "]";
		string colUniName = "lightCol[" + countStr + "]";

		glm::vec3 dir(iter->second.dir);
		glm::vec3 col(iter->second.color);

		_shaderProgram->SetUniform3f(dirUniName.c_str(), dir.x, dir.y, dir.z);
		_shaderProgram->SetUniform3f(colUniName.c_str(), col.x, col.y, col.z);

		count++;
	}

	_shaderProgram->SetUniform1i("numLights", (int)_lightMap.size());
	_shaderProgram->SetUniform1f("metallic", _metallic);
	_shaderProgram->SetUniform1f("roughness", _roughness);
	_shaderProgram->SetUniform1f("alpha", _alpha);

	if (_colorBufferID)
	{
		GLuint loc = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
		glEnableVertexAttribArray(loc);
		glBindBuffer(GL_ARRAY_BUFFER, _colorBufferID);
		glVertexAttribPointer(loc, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, (void*)0);
	}

	if (_normalBufferID)
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

void PBRCubeGeometryShader::End()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_colorBufferID)
	{
		GLuint loc = glGetAttribLocation(_shaderProgram->ProgramID(), "color");
		glDisableVertexAttribArray(loc);
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

PBRCubeGeometryShader::~PBRCubeGeometryShader()
{
}