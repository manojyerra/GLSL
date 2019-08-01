#ifndef ShaderProgramsManager_H
#define ShaderProgramsManager_H

#include "ShaderProgram.h"
#include <string>
#include <map>

using namespace std;

class ShaderInfo
{
private:
	ShaderProgram* _shaderProgram;
	int _refCount;

public:
	ShaderInfo(ShaderProgram* shaderProgram)
	{
		_shaderProgram = shaderProgram;
		_refCount = 1;
	}

	~ShaderInfo()
	{
	}

	int GetRefCount()
	{
		return _refCount;
	}

	void IncreaseRefCount()
	{
		_refCount++;
	}

	void DecreaseRefCount()
	{
		_refCount--;

		if (_refCount < 0)
		{
			return;  //Reference count should not negative value.
		}
	}

	ShaderProgram* GetShaderProgram()
	{
		return _shaderProgram;
	}
};

class ShaderProgramsManager
{
private:
	static ShaderProgramsManager* _ref;
	map<string, ShaderInfo*> shadersMap;

	ShaderProgramsManager();
	~ShaderProgramsManager();

public:
	static ShaderProgramsManager* GetInstance();

	ShaderProgram* CreateShaderProgram(string vertexShader, string fragmentShader);
	ShaderProgram* CreateShaderProgram(string vertexShader, string geometryShader, string fragmentShader);

	void DeleteShaderProgram(ShaderProgram* shaderProgram);

	static void DeleteInstance();
};

#endif