#include "ParticleRenderer.h"
#include "ParticleReader.h"

ParticleRenderer::ParticleRenderer()
{
	_modelMat.SetRotation(glm::vec3(0,90,90));

	ParticleReader particleReader("data/demo/Particles");

	_normalPolyRenderer = new GLMeshRenderer(&particleReader, GLMeshRenderer::CUBE_GEOMETRY_SHADER);
	_normalPolyRenderer->SetPrimitiveType(GLMeshRenderer::points);

	particleReader.SetRetriveDataType(ParticleReader::DATA_AS_LOW_POLY);

	_lowPolyRenderer = new GLMeshRenderer(&particleReader, GLMeshRenderer::CUBE_GEOMETRY_SHADER);
	_lowPolyRenderer->SetPrimitiveType(GLMeshRenderer::points);
}

void ParticleRenderer::SetPosition(float x, float y, float z)
{
	_modelMat.SetPos(x, y, z);
}

void ParticleRenderer::DrawAllParticles()
{
	_normalPolyRenderer->SetModelMatrix(_modelMat.m);
	_normalPolyRenderer->Draw();
}

void ParticleRenderer::DrawLowPolyParticles()
{
	_lowPolyRenderer->SetModelMatrix(_modelMat.m);
	_lowPolyRenderer->Draw();
}

ParticleRenderer::~ParticleRenderer()
{
	if(_normalPolyRenderer)
	{
		delete _normalPolyRenderer;
		_normalPolyRenderer = NULL;
	}

	if (_lowPolyRenderer)
	{
		delete _lowPolyRenderer;
		_lowPolyRenderer = NULL;
	}
}
