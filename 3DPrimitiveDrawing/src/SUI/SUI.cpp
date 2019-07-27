#ifdef _ENABLE_SUI
#include "SUI.h"

void SUISetup(float windowWidth, float windowHeight)
{
	SUIManager::GetInstance()->Setup(windowWidth, windowHeight);
}

void SUISetWindowSize(float windowWidth, float windowHeight)
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

#endif