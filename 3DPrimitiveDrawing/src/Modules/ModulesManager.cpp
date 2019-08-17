#include "ModulesManager.h"
#include "Cam.h"
#include "Cam2D.h"
#include "ShaderProgramsManager.h"
#include "GLMemory.h"
#include "SUI.h"
#include "GLState.h"
#include "Input.h"
#include "GLFBOManager.h"

ModulesManager* ModulesManager::_ref = nullptr;

ModulesManager::ModulesManager()
{
	_sw = 0;
	_sh = 0;

	_argc = 0;;
	_argv = nullptr;

	_demo = nullptr;
	_ecoatPost = nullptr;

	_module = NULL;
	_moduleID = 0;
}

void ModulesManager::Init(unsigned int sw, unsigned int sh, int argc, char** argv, unsigned int moduleID)
{
	_sw = sw;
	_sh = sh;

	_argc = argc;
	_argv = argv;

	GLFBOManager::GetInstance()->Init(sw, sh);

	SetModule(moduleID);
	SUISetup(sw, sh);
}

ModulesManager* ModulesManager::GetInstance()
{
	if (_ref == nullptr)
	{
		_ref = new ModulesManager();
	}

	return _ref;
}

void ModulesManager::SetModule(unsigned int moduleID)
{
	_moduleID = moduleID;

	if (_moduleID == DEMO_MODULE)
	{
		if(!_demo)
		{
			_demo = new Demo(_sw, _sh);
		}

		_module = _demo;
	}
	else if (_moduleID == ECOAT_POST_MODULE)
	{
		if (!_ecoatPost)
		{
			_ecoatPost = new ECoatPost(_sw, _sh, _argc, _argv);
		}

		_module = _ecoatPost;
	}
	else
	{
		throw new std::exception("Error : Invalid module ID.");
	}

	//TODO: Implement onModuleChange method in every module. 
	//Take care of camera and screen size changes.
}

void ModulesManager::SetScreenSize(unsigned int sw, unsigned int sh)
{
	_sw = sw;
	_sh = sh;

	if (_module)
	{
		_module->SetScreenSize(sw, sh);
	}
}

void ModulesManager::Update(float deltaTime)
{
	if (_module)
	{
		_module->Update(deltaTime);
	}	
}

void ModulesManager::Draw()
{
	SUIInput::SCROLL_STATE_STORE = Input::SCROLL_STATE;
	bool consumed = SUIInput::Update((float)Input::MX, (float)Input::MY, Input::LEFT_BUTTON_DOWN, 1.0f / 30.0f);
	Input::SetEnable(!consumed);

	if (_module)
	{
		_module->Draw();
	}

	bool multiSample = GLState::GLEnable(GL_MULTISAMPLE, false);
	SUIDraw();
	GLState::GLEnable(GL_MULTISAMPLE, multiSample);
}

void ModulesManager::DeleteInstance()
{
	if (_ref)
	{
		delete _ref;
		_ref = nullptr;
	}
}

ModulesManager::~ModulesManager()
{
	if (_demo)
	{
		delete _demo;
		_demo = nullptr;
	}

	if (_ecoatPost)
	{
		delete _ecoatPost;
		_ecoatPost = nullptr;
	}

	_module = nullptr;

	SUIQuit();

	Cam::DeleteInstance();
	Cam2D::DeleteInstance();

	GLFBOManager::DeleteInstance();
	ShaderProgramsManager::DeleteInstance();
	GLMemory::printMemoryLeaks();
}
