#include "ModulesManager.h"
#include "Cam.h"
#include "Cam2D.h"
#include "ShaderProgramsManager.h"
#include "GLMemory.h"

ModulesManager* ModulesManager::_ref = nullptr;

ModulesManager::ModulesManager()
{
	_sw = 0;
	_sh = 0;

	_module = NULL;
	_moduleID = 0;
}

void ModulesManager::Init(unsigned int sw, unsigned int sh, unsigned int moduleID)
{
	_sw = sw;
	_sh = sh;

	SetModule(moduleID);
}

ModulesManager* ModulesManager::GetInstance()
{
	if (_ref == nullptr)
	{
		_ref = new ModulesManager();
	}

	return _ref;
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
	else
	{
		throw new std::exception("Error : Invalid module ID.");
	}

	//TODO: Implement onModuleChange method in every module. 
	//Take care of camera and screen size changes.
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
	if (_module)
	{
		_module->Draw();
	}
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
		_demo = NULL;
	}

	_module = NULL;

	Cam::DeleteInstance();
	Cam2D::DeleteInstance();

	ShaderProgramsManager::GetInstance()->DeleteInstance();
	GLMemory::printMemoryLeaks();
}
