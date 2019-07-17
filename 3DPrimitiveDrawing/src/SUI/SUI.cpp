#include "SUI.h"

void SUISetup(int windowWidth, int windowHeight)
{
	SUIManager::GetInstance()->Setup(windowWidth, windowHeight);
}

void SUISetWindowSize(int windowWidth, int windowHeight)
{
	SUIManager::GetInstance()->SetWindowSize(windowWidth, windowHeight);
}

void SUIDraw()
{
	SUIManager::GetInstance()->Run();
}

void SUIQuit()
{
	SUIManager::GetInstance()->DeleteInstance();
}
