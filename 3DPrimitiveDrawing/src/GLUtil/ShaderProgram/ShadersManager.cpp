#include "ShadersManager.h"

ShadersManager* ShadersManager::_ref = NULL;

ShadersManager::ShadersManager()
{

}

ShadersManager::~ShadersManager()
{

}

ShadersManager* ShadersManager::GetInstance()
{
	if (_ref == NULL)
		_ref = new ShadersManager();

	return _ref;
}

ShaderProgram* ShadersManager::CreateShaderProgram(string vertexShader, string fragmentShader)
{
	string key = "";
	key.append(vertexShader).append(":").append(fragmentShader);

	ShaderProgram* shaderProgram = NULL;

	if (shadersMap.find(key) != shadersMap.end())
	{
		ShaderInfo* shaderInfo = shadersMap.find(key)->second;
		shaderInfo->IncreaseRefCount();
		shaderProgram = shaderInfo->GetShaderProgram();
	}
	else
	{
		shaderProgram = new ShaderProgram(vertexShader.c_str(), fragmentShader.c_str());
		ShaderInfo* shaderInfo = new ShaderInfo(shaderProgram);
		shadersMap[key] = shaderInfo;
	}

	return shaderProgram;
}

ShaderProgram* ShadersManager::CreateShaderProgram(string vertexShader, string geometryShader, string fragmentShader)
{
	string key = "";
	key.append(vertexShader).append(":").append(geometryShader).append(":").append(fragmentShader);

	ShaderProgram* shaderProgram = NULL;

	if (shadersMap.find(key) != shadersMap.end())
	{
		ShaderInfo* shaderInfo = shadersMap.find(key)->second;
		shaderInfo->IncreaseRefCount();
		shaderProgram = shaderInfo->GetShaderProgram();
	}
	else
	{
		shaderProgram = new ShaderProgram(vertexShader.c_str(), geometryShader.c_str(), fragmentShader.c_str());
		ShaderInfo* shaderInfo = new ShaderInfo(shaderProgram);
		shadersMap[key] = shaderInfo;
	}

	return shaderProgram;
}

void ShadersManager::DeleteShaderProgram(ShaderProgram* shaderProgram)
{
	string vertexShader = shaderProgram->GetVertexShaderFilePath();
	string geometryShader = shaderProgram->GetGeometryShaderFilePath();
	string fragmentShader = shaderProgram->GetFragmentShaderFilePath();

	string key = "";
	
	if(shaderProgram->HasGeometryShader())
	{
		key.append(vertexShader).append(":").append(geometryShader).append(":").append(fragmentShader);
	}
	else
	{
		key.append(vertexShader).append(":").append(fragmentShader);
	}

	if (shadersMap.find(key) != shadersMap.end())
	{
		ShaderInfo* shaderInfo = shadersMap.find(key)->second;
		shaderInfo->DecreaseRefCount();

		if (shaderInfo->GetRefCount() == 0)
		{
			delete shaderInfo->GetShaderProgram();
			delete shaderInfo;
			shadersMap.erase(shadersMap.find(key));
		}
	}
}

void ShadersManager::DeleteInstance()
{
	if (_ref)
	{
		delete _ref;
		_ref = NULL;
	}
}
