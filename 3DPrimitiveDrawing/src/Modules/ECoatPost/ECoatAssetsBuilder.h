#ifndef ECoatAssetsBuilder_H
#define ECoatAssetsBuilder_H

#include "ECoatAssetsReader.h"
#include "GLMeshRenderer.h"
#include "ECoatResultReader.h"
#include "GLMeshManager.h"

class ECoatAssetsBuilder
{
private:
	GLMeshRenderer* _solid;
	GLMeshRenderer* _fluid;
	std::vector<GLMeshRenderer*> _sourcesVec;
	ECoatResultReader* _resultReader;

public:
	ECoatAssetsBuilder(ECoatAssetsReader* assetsReader, GLMeshManager* meshMgr);
	~ECoatAssetsBuilder();

	GLMeshRenderer* GetSolid();
	GLMeshRenderer* GetFluid();
	std::vector<GLMeshRenderer*> GetSources();
	ECoatResultReader* GetResultReader();
};

#endif