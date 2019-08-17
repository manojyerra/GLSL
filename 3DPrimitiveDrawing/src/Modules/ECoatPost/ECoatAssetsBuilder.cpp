#include "ECoatAssetsBuilder.h"
#include "STLReader.h"
#include "GLMeshManager.h"

ECoatAssetsBuilder::ECoatAssetsBuilder(ECoatAssetsReader* assetsReader, GLMeshManager* meshMgr)
{
	_solid = nullptr;
	_fluid = nullptr;
	_sourcesVec.clear();
	_resultReader = nullptr;

	std::string solidPath = assetsReader->GetSolid();
	if (solidPath.length() > 0)
	{
		_solid = meshMgr->AddMeshRenderer(solidPath, PBR_SHADER, BaseModelIO::STL_MODEL_WITH_THREADS);
	}

	std::vector<std::string> sourcesPathVec = assetsReader->GetSources();
	for (int i = 0; i < (int)sourcesPathVec.size(); i++)
	{
		GLMeshRenderer* meshRenderer = meshMgr->AddMeshRenderer(sourcesPathVec[i], PBR_SHADER, BaseModelIO::STL_MODEL_WITH_THREADS);
		_sourcesVec.push_back(meshRenderer);
	}

	std::string fluidPath = assetsReader->GetFluid();
	if (fluidPath.length() > 0)
	{
		_fluid = meshMgr->AddMeshRenderer(fluidPath, BASIC_SHADER, BaseModelIO::STL_MODEL_WITH_THREADS);
		_fluid->SetAlpha(0.4f);
	}

	std::string resultPath = assetsReader->GetResult();
	if (resultPath.length() > 0)
	{
		_resultReader = new ECoatResultReader(resultPath);
	}

	//std::string resultPath = "result_fine.ecoat";
	//if (resultPath.length() > 0)
	//{
	//	_resultReader = new ECoatResultReader(resultPath);
	//}
}

GLMeshRenderer* ECoatAssetsBuilder::GetSolid()
{
	return _solid;
}

GLMeshRenderer* ECoatAssetsBuilder::GetFluid()
{
	return _fluid;
}

std::vector<GLMeshRenderer*> ECoatAssetsBuilder::GetSources()
{
	return _sourcesVec;
}

ECoatResultReader* ECoatAssetsBuilder::GetResultReader()
{
	return _resultReader;
}

ECoatAssetsBuilder::~ECoatAssetsBuilder()
{
	if (_resultReader)
	{
		delete _resultReader;
		_resultReader = nullptr;
	}
}