#include "ParticleRenderer.h"
#include "ParticleReader.h"

ParticleRenderer::ParticleRenderer()
{
	_modelMat.SetRotation(glm::vec3(0,90,90));

	ParticleReader particleReader("data/BigSize/Particles");

	_allParticlesRenderer = new GLMeshRenderer(&particleReader, GLMeshRenderer::CUBE_GEOMETRY_SHADER);
	_allParticlesRenderer->SetPrimitiveType(GLMeshRenderer::points);

	particleReader.SetRetriveDataType(ParticleReader::DATA_AS_LOW_POLY);

	_fewParticlesRenderer = new GLMeshRenderer(&particleReader, GLMeshRenderer::CUBE_GEOMETRY_SHADER);
	_fewParticlesRenderer->SetPrimitiveType(GLMeshRenderer::points);
}

void ParticleRenderer::SetPosition(float x, float y, float z)
{
	_modelMat.SetPos(x, y, z);
}

void ParticleRenderer::DrawAllParticles()
{
	_allParticlesRenderer->SetModelMatrix(_modelMat.m);
	_allParticlesRenderer->Draw();
}

void ParticleRenderer::DrawLowPolyParticles()
{
	_fewParticlesRenderer->SetModelMatrix(_modelMat.m);
	_fewParticlesRenderer->Draw();
}

ParticleRenderer::~ParticleRenderer()
{
	if(_allParticlesRenderer)
	{
		delete _allParticlesRenderer;
		_allParticlesRenderer = NULL;
	}

	if (_fewParticlesRenderer)
	{
		delete _fewParticlesRenderer;
		_fewParticlesRenderer = NULL;
	}
}
