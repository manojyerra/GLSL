#include "ParticleRenderer.h"

ParticleRenderer::ParticleRenderer()
{
	_modelMat.SetRotation(glm::vec3(0,90,90));

	BinaryObjReader binaryObjReader("data/BigSize/Particles");

	_allParticlesRenderer = CreateAllParticlesRenderer(&binaryObjReader);
	_fewParticlesRenderer = CreateFewParticlesRenderer(&binaryObjReader);
}

GLMeshRenderer* ParticleRenderer::CreateAllParticlesRenderer(BinaryObjReader* binaryObjReader)
{
	// Generating color buffer.
	unsigned int fileLen = binaryObjReader->GetVertexBufferSize();
	unsigned int vertexCount = fileLen / 12;

	unsigned int colorBufLen = vertexCount * 3;
	unsigned char* colorBuf = (unsigned char*)malloc(colorBufLen);
	memset(colorBuf, 0, colorBufLen);

	for (unsigned int i = 0; i < vertexCount; i++)
	{
		colorBuf[i * 3 + 0] = 200;
		//colorBuf[i*3 + 1] = 0;
		//colorBuf[i*3 + 2] = 0;
	}

	BaseModelIO modelIO;
	modelIO.SetVertexBuffer(binaryObjReader->GetVertexBuffer(), binaryObjReader->GetVertexBufferSize());
	modelIO.SetColorBuffer((const char*)colorBuf, colorBufLen);

	GLMeshRenderer* meshRenderer = new GLMeshRenderer(&modelIO, CUBE_GEOMETRY_SHADER);
	meshRenderer->SetPrimitiveType(GLMeshRenderer::points);

	free(colorBuf);

	return meshRenderer;
}

GLMeshRenderer* ParticleRenderer::CreateFewParticlesRenderer(BinaryObjReader* binaryObjReader)
{
	const unsigned int BYTES_PER_VERTEX = 12;
	const unsigned int skipNumVertex = 50;

	unsigned int length = binaryObjReader->GetVertexBufferSize();
	char* allParticleBuf = (char*)binaryObjReader->GetVertexBuffer();

	//Generating low poly vertex data
	unsigned int bpv = BYTES_PER_VERTEX;
	unsigned int skipBytes = skipNumVertex * bpv;
	unsigned int bufLen =  length / (skipNumVertex - 1);

	unsigned char* vertexBuf = (unsigned char*)malloc(bufLen);

	unsigned int vertexCount = 0;

	for (unsigned int i = 0, j = 0; i < length - skipBytes; i += skipBytes, j += bpv)
	{
		memcpy(&vertexBuf[j], &allParticleBuf[i], bpv);
		vertexCount++;
	}

	unsigned int vertexBufLen = vertexCount * bpv;

	//Generating low poly color data
	unsigned int colorBufLen = vertexCount * 3;
	unsigned char* colorBuf = (unsigned char*)malloc(colorBufLen);
	memset(colorBuf, 0, colorBufLen);

	for (unsigned int i = 0; i < vertexCount; i++)
	{
		colorBuf[i*3 + 0] = 200;
		//colorBuf[i*3 + 1] = 0;
		//colorBuf[i*3 + 2] = 0;
	}

	BaseModelIO modelIO;
	modelIO.SetVertexBuffer((const char*)vertexBuf, vertexBufLen);
	modelIO.SetColorBuffer((const char*)colorBuf, colorBufLen);

	GLMeshRenderer* meshRenderer = new GLMeshRenderer(&modelIO, CUBE_GEOMETRY_SHADER);
	meshRenderer->SetPrimitiveType(GLMeshRenderer::points);

	free(vertexBuf);
	free(colorBuf);

	return meshRenderer;
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

void ParticleRenderer::DrawFewParticles()
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
