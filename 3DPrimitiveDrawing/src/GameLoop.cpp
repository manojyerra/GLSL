#include "GameLoop.h"
#include "ModulesManager.h"
#include "FileReader.h"

GameLoop::GameLoop(float sw, float sh, int argc, char** argv)
{
	ModulesManager::GetInstance()->Init(sw, sh, argc, argv, ModulesManager::ECOAT_POST_MODULE);
}

void GameLoop::Update(float deltaTime)
{
	ModulesManager::GetInstance()->Update(deltaTime);
}

void GameLoop::Draw()
{
	ModulesManager::GetInstance()->Draw();
}

void GameLoop::SetScreenSize(unsigned int sw, unsigned int sh)
{
	ModulesManager::GetInstance()->SetScreenSize(sw, sh);
}

GameLoop::~GameLoop()
{
	ModulesManager::DeleteInstance();
}


//glm::vec3 v1(0.0f, 0.0f, 0.0f);
//glm::vec3 v2(0.0f, 5.0f, 0.0f);
//glm::vec3 v3(5.0f, 0.0f, 0.0f);

//_triangle = new Triangle(v1, v2, v3);

//_box = new Box(0, 0, 0, 2, 3, 4);
//_box->SetSize(3, 1, 6);
//_box->SetPos(-10, 0, -10);

//_cylinder = new Cylinder(0, 0, 0, 3, 2);
//_cylinder->SetRadius(1.5);
//_cylinder->SetHeight(2);
//_cylinder->SetPos(0, 0, -10);

//_cone = new Cone(0, 0, 0, 2, 3);
//_cone->SetRadius(1.5);
//_cone->SetHeight(2);
//_cone->SetPos(-5, 0, -10);

//_sphere = new Sphere(0, 0, 0, 2);
//_sphere->SetPos(5, 0, 0);
//_sphere->SetRadius(5);
