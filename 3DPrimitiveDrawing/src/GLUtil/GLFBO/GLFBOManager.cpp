#include "GLFBOManager.h"
#include "GLMemory.h"

GLFBOManager* GLFBOManager::_ref = nullptr;

GLFBOManager::GLFBOManager()
{
	_sw = 0;
	_sh = 0;
	_defaultFBO = nullptr;
	_defaultMSFBO = nullptr;
}

GLFBOManager* GLFBOManager::GetInstance()
{
	if (_ref == nullptr)
	{
		_ref = new GLFBOManager();
	}

	return _ref;
}

void GLFBOManager::Init(int sw, int sh)
{
	_sw = sw;
	_sh = sh;

	_defaultFBO = new GLFBO(_sw, _sh);
	_defaultMSFBO = new GLMSFBO(_sw, _sh);
}

void GLFBOManager::SetScreenSize(int sw, int sh)
{
	if (sw != _sw || sh != _sh)
	{
		_sw = sw;
		_sh = sh;

		if (_defaultFBO)
		{
			delete _defaultFBO;
			_defaultFBO = nullptr;
		}

		_defaultFBO = new GLFBO(_sw, _sh);

		if (_defaultMSFBO)
		{
			delete _defaultMSFBO;
			_defaultMSFBO = new GLMSFBO(_sw, _sh);
		}
	}
}

void GLFBOManager::BindDefaultFBO()
{
	_defaultFBO->BindFBO();
}

void GLFBOManager::UnBindDefaultFBO()
{
	_defaultFBO->UnBindFBO();
}

unsigned int GLFBOManager::GetDefaultFBOTexID()
{
	return _defaultFBO->GetTextureID();
}

void GLFBOManager::BindDefaultMSFBO()
{
	_defaultMSFBO->BindFBO();
}

void GLFBOManager::UnBindDefaultMSFBO()
{
	if (_defaultMSFBO)
		_defaultMSFBO->UnBindFBO();
}

unsigned int GLFBOManager::GetDefaultMSFBOTexID()
{
	return _defaultMSFBO->GetTextureID();
}

void GLFBOManager::DeleteInstance()
{
	if (_ref)
	{
		delete _ref;
		_ref = nullptr;
	}
}

GLFBOManager::~GLFBOManager()
{
	if (_defaultFBO)
	{
		delete _defaultFBO;
		_defaultFBO = nullptr;
	}

	if (_defaultMSFBO)
	{
		delete _defaultMSFBO;
		_defaultMSFBO = nullptr;
	}
}
