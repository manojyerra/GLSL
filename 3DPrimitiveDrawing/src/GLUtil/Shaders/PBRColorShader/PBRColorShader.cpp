#include "PBRColorShader.h"
#include "ShaderProgramsManager.h"
#include "Cam.h"

PBRColorShader::PBRColorShader() : PBRShader("shaders/PBRColorShader/PBRColorShader.vs",
	"shaders/PBRColorShader/PBRColorShader.fs")
{
	_metallic = 1.0f;
	_roughness = 0.23f;
}

void PBRColorShader::Begin()
{
	_shaderProgram->Begin();
}

void PBRColorShader::SetUniformsAndAttributes()
{
	Cam* cam = Cam::GetInstance();
	float* m = _modelMat.m;

	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam->GetMVP(m)));
	_shaderProgram->SetUniformMatrix4fv("modelViewMat", glm::value_ptr(cam->GetModelViewMat(m)));
	_shaderProgram->SetUniformMatrix3fv("normalMat", glm::value_ptr(cam->GetNormalMat(m)));

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

void PBRColorShader::End()
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

PBRColorShader::~PBRColorShader()
{
}