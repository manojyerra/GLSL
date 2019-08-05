#ifndef ModulesManager_H
#define ModulesManager_H

#include "Module.h"
#include "Demo.h"
#include "ECoatPost.h"

class ModulesManager
{
private:
	unsigned int _sw;
	unsigned int _sh;
	int _argc;
	char** _argv;

	Demo* _demo;
	ECoatPost* _ecoatPost;

	Module* _module;
	unsigned  int _moduleID;
	static ModulesManager* _ref;

	ModulesManager();
	~ModulesManager();

public:
	enum
	{
		DEMO_MODULE,
		ECOAT_POST_MODULE
	};

	void Init(unsigned int sw, unsigned int sh, int argc, char** argv, unsigned int moduleID);
	static ModulesManager* GetInstance();

	void SetModule(unsigned int moduleID);

	void SetScreenSize(unsigned int sw, unsigned int sh);
	void Update(float deltaTime);
	void Draw();

	static void DeleteInstance();
};

#endif
