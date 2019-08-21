#include "ParticleRenderer.h"
#include "Cam.h"
#include "BufferTransformUtils.h"

ParticleRenderer::ParticleRenderer(std::string filePath)
{
	_skipNumVertex = 50;

	BinaryObjReader binaryObjReader(filePath);

	char* vertexBuf = (char*)binaryObjReader.GetVertexBuffer();
	unsigned int vertexBufLen = binaryObjReader.GetVertexBufferSize();

	_bBoxCenter = BufferTransformUtils::CalcCenter((float*)vertexBuf, vertexBufLen / 4);

	_allParticlesRenderer = CreateAllParticlesRenderer(&BufferInfo(vertexBuf, vertexBufLen));
	_fewParticlesRenderer = CreateFewParticlesRenderer(&BufferInfo(vertexBuf, vertexBufLen));
}

ParticleRenderer::ParticleRenderer(BufferInfo* vertexBufInfo)
{
	_skipNumVertex = 50;
	_bBoxCenter = BufferTransformUtils::CalcCenter((float*)vertexBufInfo->buffer, vertexBufInfo->size / 4);

	_allParticlesRenderer = CreateAllParticlesRenderer(vertexBufInfo);
	_fewParticlesRenderer = CreateFewParticlesRenderer(vertexBufInfo);
}

ParticleRenderer::ParticleRenderer(BufferInfo* vertexBufInfo, BufferInfo* normalBufInfo)
{
	_skipNumVertex = 50;
	_bBoxCenter = BufferTransformUtils::CalcCenter((float*)vertexBufInfo->buffer, vertexBufInfo->size / 4);

	_allParticlesRenderer = CreateAllParticlesRenderer(vertexBufInfo, normalBufInfo);
	_fewParticlesRenderer = CreateFewParticlesRenderer(vertexBufInfo, normalBufInfo);
}

void ParticleRenderer::SetDrawAs(int drawAs)
{
	if (drawAs == DRAW_AS_POINTS)
	{
		_allParticlesRenderer->SetShader(COLOR_SHADER);
		_fewParticlesRenderer->SetShader(COLOR_SHADER);
	}
	else if (drawAs == DRAW_AS_CUBES)
	{
		_allParticlesRenderer->SetShader(CUBE_GEOMETRY_SHADER);
		_fewParticlesRenderer->SetShader(CUBE_GEOMETRY_SHADER);
	}
	else if (drawAs == DRAW_AS_SPHERES)
	{
		_allParticlesRenderer->SetShader(SPHERE_GEOMETRY_SHADER);
		_fewParticlesRenderer->SetShader(SPHERE_GEOMETRY_SHADER);
	}
	else if (drawAs == DRAW_AS_QUADS)
	{
		_allParticlesRenderer->SetShader(QUAD_GEOMETRY_SHADER);
		_fewParticlesRenderer->SetShader(QUAD_GEOMETRY_SHADER);
	}
}

GLMeshRenderer* ParticleRenderer::CreateAllParticlesRenderer(BufferInfo* allParVerBufInfo)
{
	unsigned int colorBufLen = allParVerBufInfo->size /4;
	unsigned char* colorBuf = (unsigned char*)malloc(colorBufLen);
	memset(colorBuf, 128, colorBufLen);

	BaseModelIO modelIO;
	modelIO.SetVertexBuffer(allParVerBufInfo->buffer, allParVerBufInfo->size);
	modelIO.SetColorBuffer((const char*)colorBuf, colorBufLen);

	GLMeshRenderer* meshRenderer = new GLMeshRenderer(&modelIO, CUBE_GEOMETRY_SHADER);
	meshRenderer->SetPrimitiveType(GLMeshRenderer::points);

	free(colorBuf);

	return meshRenderer;
}

GLMeshRenderer* ParticleRenderer::CreateFewParticlesRenderer(BufferInfo* highPolyVerBufInfo)
{
	//Generating low poly vertex data
	unsigned int bytesPVer = BYTES_PER_VERTEX;
	
	unsigned int highPolyVerCount = highPolyVerBufInfo->size / bytesPVer;
	unsigned int lowPolyVerCount = highPolyVerCount / (_skipNumVertex+1);
	unsigned int lowPolyVerBufLen = lowPolyVerCount * bytesPVer;
	unsigned char* lowPolyVertexBuf = (unsigned char*)malloc(lowPolyVerBufLen);

	unsigned int skipBytes = (_skipNumVertex+1) * bytesPVer;
	unsigned int hIndex = 0; // high poly array index

	char* highPolyVerBuf = highPolyVerBufInfo->buffer;

	for (unsigned int i = 0; i < lowPolyVerBufLen; i += bytesPVer)
	{
		memcpy(&lowPolyVertexBuf[i], &highPolyVerBuf[hIndex], bytesPVer);
		hIndex += skipBytes;
	}

	//Generating low poly color data
	unsigned int lowPolyColorBufLen = lowPolyVerCount * BYTES_PER_COLOR;
	unsigned char* lowPolyColorBuf = (unsigned char*)malloc(lowPolyColorBufLen);
	memset(lowPolyColorBuf, 255, lowPolyColorBufLen);

	BaseModelIO modelIO;
	modelIO.SetVertexBuffer((const char*)lowPolyVertexBuf, lowPolyVerBufLen);
	modelIO.SetColorBuffer((const char*)lowPolyColorBuf, lowPolyColorBufLen);

	GLMeshRenderer* meshRenderer = new GLMeshRenderer(&modelIO, CUBE_GEOMETRY_SHADER);
	meshRenderer->SetPrimitiveType(GLMeshRenderer::points);

	free(lowPolyVertexBuf);
	free(lowPolyColorBuf);

	return meshRenderer;
}

GLMeshRenderer* ParticleRenderer::CreateAllParticlesRenderer(BufferInfo* vertexBufInfo, BufferInfo* normalBufInfo)
{
	unsigned int colorBufLen = vertexBufInfo->size / 4;
	unsigned char* colorBuf = (unsigned char*)malloc(colorBufLen);
	memset(colorBuf, 128, colorBufLen);

	BaseModelIO modelIO;
	modelIO.SetVertexBuffer(vertexBufInfo->buffer, vertexBufInfo->size);
	modelIO.SetNormalBuffer(normalBufInfo->buffer, normalBufInfo->size);
	modelIO.SetColorBuffer((const char*)colorBuf, colorBufLen);

	GLMeshRenderer* meshRenderer = new GLMeshRenderer(&modelIO, CUBE_GEOMETRY_SHADER);
	meshRenderer->SetPrimitiveType(GLMeshRenderer::points);

	free(colorBuf);

	return meshRenderer;
}

GLMeshRenderer* ParticleRenderer::CreateFewParticlesRenderer(BufferInfo* allParVerBufInfo, BufferInfo* normalBufInfo)
{
	return NULL;
}

void ParticleRenderer::SetPosition(float x, float y, float z)
{
	_modelMat.SetPos(x, y, z);
}

glm::vec3 ParticleRenderer::GetBBoxCenter()
{
	return _bBoxCenter;
}

void ParticleRenderer::UpdateColorBuffer(char* highPolyColorBuf, unsigned int highPolyColorBufLen)
{
	_allParticlesRenderer->UpdateColorBuffer(highPolyColorBuf, highPolyColorBufLen);
	
	unsigned int bytesPColor = BYTES_PER_COLOR;
	unsigned int highPolyColorCount = highPolyColorBufLen / bytesPColor;
	unsigned int lowPolyColorCount = highPolyColorCount / (_skipNumVertex + 1);
	unsigned int lowPolyColorBufLen = lowPolyColorCount * bytesPColor;
	unsigned char* lowPolyColorBuf = (unsigned char*)malloc(lowPolyColorBufLen);

	unsigned int skipBytes = (_skipNumVertex+1) * bytesPColor;
	unsigned int hIndex = 0; // high poly array index

	for (unsigned int i = 0; i < lowPolyColorBufLen; i += bytesPColor)
	{
		memcpy(&lowPolyColorBuf[i], &highPolyColorBuf[hIndex], bytesPColor);
		hIndex += skipBytes;
	}

	_fewParticlesRenderer->UpdateColorBuffer(lowPolyColorBuf, lowPolyColorBufLen);

	free(lowPolyColorBuf);
}

void ParticleRenderer::DrawAllParticles()
{
	if(_allParticlesRenderer)
	{
		_allParticlesRenderer->SetModelMatrix(_modelMat.m);
		_allParticlesRenderer->Draw();
	}
}

void ParticleRenderer::DrawFewParticles()
{
	if(_fewParticlesRenderer)
	{
		_fewParticlesRenderer->SetModelMatrix(_modelMat.m);
		_fewParticlesRenderer->Draw();
	}
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


//GLMeshRenderer* ParticleRenderer::CreateAllParticlesRenderer(char* vertexBuf, unsigned int vertexBufLen, char* colorBuf, unsigned int colorBufLen)
//{
//	BaseModelIO modelIO;
//	modelIO.SetVertexBuffer(vertexBuf, vertexBufLen);
//	modelIO.SetColorBuffer((const char*)colorBuf, colorBufLen);
//
//	GLMeshRenderer* meshRenderer = new GLMeshRenderer(&modelIO, CUBE_GEOMETRY_SHADER);
//	meshRenderer->SetPrimitiveType(GLMeshRenderer::points);
//
//	return meshRenderer;
//}


//ParticleRenderer::ParticleRenderer(char* vertexBuf, unsigned int vertexBufLen, char* colorBuf, unsigned int colorBufLen)
//{
//	_modelMat.SetRotation(glm::vec3(0, 90, 90));
//
//	_allParticlesRenderer = CreateAllParticlesRenderer(vertexBuf, vertexBufLen, colorBuf, colorBufLen);
//	_fewParticlesRenderer = CreateFewParticlesRenderer(vertexBuf, vertexBufLen);
//}