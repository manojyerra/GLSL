#ifndef ECoatAssetsBuilder_H
#define ECoatAssetsBuilder_H

#include "ECoatAssetsReader.h"
#include "GLMeshRenderer.h"
#include "ECoatResultReader.h"
#include "GLMeshManager.h"
#include "STLReaderWithThreads.h"

class ECoatAssetsBuilder
{
private:
	GLMeshRenderer* _solid;
	STLReaderWithThreads* _solidSTLReader;
	GLMeshRenderer* _fluid;
	std::vector<GLMeshRenderer*> _sourcesVec;
	ECoatResultReader* _resultReader;

public:
	ECoatAssetsBuilder(ECoatAssetsReader* assetsReader, GLMeshManager* meshMgr);
	~ECoatAssetsBuilder();

	GLMeshRenderer* GetSolid();
	BaseModelIO* GetSolidSTLReader();
	GLMeshRenderer* GetFluid();
	std::vector<GLMeshRenderer*> GetSources();
	ECoatResultReader* GetResultReader();
};

#endif