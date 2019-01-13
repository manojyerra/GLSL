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
	string key = vertexShader;
	key.append(":").append(fragmentShader);

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

void ShadersManager::DeleteShaderProgram(string vertexShader, string fragmentShader)
{
	string key = vertexShader.append(":").append(fragmentShader);

	if (shadersMap.find(key) != shadersMap.end())
	{
		ShaderInfo* shaderInfo = shadersMap.find(key)->second;
		shaderInfo->DecreaseRefCount();

		if (shaderInfo->GetRefCount() == 0)
		{
			delete shaderInfo;
			shadersMap.erase(shadersMap.find(key));
		}
	}
}

void ShadersManager::DeleteInstance()
{
}

