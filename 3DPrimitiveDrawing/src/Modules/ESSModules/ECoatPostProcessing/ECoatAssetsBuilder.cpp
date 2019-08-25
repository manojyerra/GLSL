#include "ECoatAssetsBuilder.h"
#include "GLMeshManager.h"

ECoatAssetsBuilder::ECoatAssetsBuilder(ECoatAssetsReader* assetsReader, GLMeshManager* meshMgr)
{
	_solid = nullptr;
	_solidSTLReader = nullptr;
	_fluid = nullptr;
	_sourcesVec.clear();
	_resultReader = nullptr;

	std::string solidPath = assetsReader->GetSolid();
	if (solidPath.length() > 0)
	{
		_solidSTLReader = new STLReaderWithThreads(solidPath);
		_solid = meshMgr->AddMeshRenderer(_solidSTLReader, PBR_SHADER);
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
		_fluid = meshMgr->AddMeshRenderer(fluidPath, PHONG_PER_VERTEX_SHADER, BaseModelIO::STL_MODEL_WITH_THREADS);
		PhongShader* phongShader = (PhongShader*)_fluid->GetCurrentShader();
		phongShader->SetDiffuseColor(1.0, 1.0f, 1.0f, 1.0f);
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

BaseModelIO* ECoatAssetsBuilder::GetSolidSTLReader()
{
	return _solidSTLReader;
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

	if (_solidSTLReader)
	{
		delete _solidSTLReader;
		_solidSTLReader = nullptr;
	}
}