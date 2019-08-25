#include "GameLoop.h"
#include "ModulesManager.h"

GameLoop::GameLoop(float sw, float sh, int argc, char** argv)
{
	ModulesManager::GetInstance()->Init(sw, sh, argc, argv, ModulesManager::ECOAT_POST_PROCESSING_MODULE);
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
