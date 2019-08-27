#include "ECoatAssetsBuilder.h"
#include "GLMeshManager.h"

ECoatAssetsBuilder::ECoatAssetsBuilder(ECoatAssetsReader* assetsReader, GLMeshManager* meshMgr)
{
	_solid = nullptr;
	_solidSTLReader = nullptr;
	_fluid = nullptr;
	_sourcesVec = nullptr;
	_resultReader = nullptr;

	_sourcesVec = new vector<GLMeshRenderer*>();

	std::string solidPath = assetsReader->GetSolid();
	if (solidPath.length() > 0)
	{
		_solidSTLReader = new STLReaderWithThreads(solidPath);
		_solid = meshMgr->AddMeshRenderer(_solidSTLReader, PBR_SHADER);
	}

	glm::vec3 albedo;
	std::vector<std::string> sourcesPathVec = assetsReader->GetSources();
	for (int i = 0; i < (int)sourcesPathVec.size(); i++)
	{
		GLMeshRenderer* meshRenderer = meshMgr->AddMeshRenderer(sourcesPathVec[i], PBR_SHADER, BaseModelIO::STL_MODEL_WITH_THREADS);
		PBRShader* pbrShader = (PBRShader*)meshRenderer->GetCurrentShader();
		pbrShader->SetMeterialProps(albedo = glm::vec3(1.0f, 0.31f, 0.41f), 1.0f, 0.20f);
		_sourcesVec->push_back(meshRenderer);
	}

	std::string fluidPath = assetsReader->GetFluid();
	if (fluidPath.length() > 0)
	{
		_fluid = meshMgr->AddMeshRenderer(fluidPath, PHONG_PER_VERTEX_SHADER, BaseModelIO::STL_MODEL_WITH_THREADS);
		PhongShader* phongShader = (PhongShader*)_fluid->GetCurrentShader();
		phongShader->SetDiffuseColor(0.5, 0.5f, 1.0f, 1.0f);
		phongShader->SetInvertNormal(true);
		_fluid->SetAlpha(0.4f);
	}

	std::string resultPath = assetsReader->GetResult();
	if (resultPath.length() > 0)
	{
		_resultReader = new ECoatResultReader(resultPath);
	}
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

std::vector<GLMeshRenderer*>* ECoatAssetsBuilder::GetSources()
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

	if (_sourcesVec)
	{
		delete _sourcesVec;
		_sourcesVec = nullptr;
	}
}


//if (i == 0) pbrShader->SetMeterialProps(albedo = glm::vec3(1.0f, 0.31f, 0.41f), 1.0f, 0.20f);
//if (i == 1) pbrShader->SetMeterialProps(albedo = glm::vec3(0.41f, 1.0f, 0.31f), 1.0f, 0.20f);
//if (i == 2) pbrShader->SetMeterialProps(albedo = glm::vec3(0.41f, 0.31f, 1.0f), 1.0f, 0.20f);
//if (i == 3) pbrShader->SetMeterialProps(albedo = glm::vec3(0.31f, 0.41f, 1.0f), 1.0f, 0.20f);
//if (i == 4) pbrShader->SetMeterialProps(albedo = glm::vec3(0.8f, 0.05f, 0.28f), 1.0f, 0.25f);
//if (i == 5) pbrShader->SetMeterialProps(albedo = glm::vec3(0.28f, 0.8f, 0.05f), 1.0f, 0.25f);
//if (i == 6) pbrShader->SetMeterialProps(albedo = glm::vec3(0.28f, 0.05f, 0.8f), 1.0f, 0.25f);
//if (i == 7) pbrShader->SetMeterialProps(albedo = glm::vec3(0.05f, 0.8f, 0.28f), 1.0f, 0.25f);
//if (i == 8) pbrShader->SetMeterialProps(albedo = glm::vec3(0.05f, 0.28f, 0.8f), 1.0f, 0.25f);
