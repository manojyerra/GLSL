#include "PBRWithTextureShader.h"
#include "ShadersManager.h"
#include "Cam.h"

PBRWithTextureShader::PBRWithTextureShader() : PBRShader("shaders/PBRWithTextgureShader/PBRWithTextgureShader.vs", 
	"shaders/PBRWithTextgureShader/PBRWithTextgureShader.fs")
{
	_uvBufferID = 0;
	_baseTexID = 0;
}

void PBRWithTextureShader::SetUVBufferID(unsigned int bufferID)
{
	_uvBufferID = bufferID;
}

void PBRWithTextureShader::SetBaseTexID(unsigned int texID)
{
	_baseTexID = texID;
}

void PBRWithTextureShader::Begin()
{
	_shaderProgram->Begin();
}

void PBRWithTextureShader::SetUniformsAndAttributes()
{
	if (_uvBufferID && _baseTexID)
	{
		glBindTexture(GL_TEXTURE_2D, _baseTexID);
	}

	Cam* cam = Cam::GetInstance();
	float* m = _modelMat.m;

	_shaderProgram->SetUniformMatrix4fv("mvp", glm::value_ptr(cam->GetMVP(m)));
	_shaderProgram->SetUniformMatrix4fv("modelViewMat", glm::value_ptr(cam->GetModelViewMat(m)));
	_shaderProgram->SetUniformMatrix3fv("normalMat", glm::value_ptr(cam->GetNormalMat(m)));

	std::map<string, LightInfo>::iterator iter;
	int count = 0;

	for(iter = _lightMap.begin(); iter != _lightMap.end(); ++iter)
	{
		string countStr = std::to_string(count);

		string dirUniName = "lightDir["+ countStr +"]";
		string colUniName = "lightCol["+ countStr +"]";

		glm::vec3 dir(iter->second.dir);
		glm::vec3 col(iter->second.color);

		_shaderProgram->SetUniform3f(dirUniName.c_str(), dir.x, dir.y, dir.z);
		_shaderProgram->SetUniform3f(colUniName.c_str(), col.x, col.y, col.z);

		count++;
	}

	_shaderProgram->SetUniform1i("numLights", _lightMap.size());

	_shaderProgram->SetUniform3f("albedo", _albedo.r, _albedo.g, _albedo.b);
	_shaderProgram->SetUniform1f("metallic", _metallic);
	_shaderProgram->SetUniform1f("roughness", _roughness);
	_shaderProgram->SetUniform1f("alpha", _alpha);

	if (_uvBufferID && _baseTexID)
	{
		GLuint uvLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "uv");
		glEnableVertexAttribArray(uvLoc);
		glBindBuffer(GL_ARRAY_BUFFER, _uvBufferID);
		glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

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

void PBRWithTextureShader::End()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_uvBufferID && _baseTexID)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		GLuint uvLoc = glGetAttribLocation(_shaderProgram->ProgramID(), "uv");
		glDisableVertexAttribArray(uvLoc);
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

PBRWithTextureShader::~PBRWithTextureShader()
{
}