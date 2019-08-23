#include "ParticleRenderer.h"
#include "Cam.h"
#include "BufferTransformUtils.h"
#include "Platform.h"

ParticleRenderer::ParticleRenderer(std::string filePath)
{
	_skipNumVertex = 50;

	BinaryObjReader binaryObjReader(filePath);

	char* vertexBuf = (char*)binaryObjReader.GetVertexBuffer();
	unsigned int vertexBufLen = binaryObjReader.GetVertexBufferSize();

	_bBoxCenter = BufferTransformUtils::CalcCenter((float*)vertexBuf, vertexBufLen / 4);

	_allParticlesRenderer = CreateAllParticlesRenderer(&BufferInfo(vertexBuf, vertexBufLen), nullptr, CUBE_GEOMETRY_SHADER);
	_fewParticlesRenderer = CreateFewParticlesRenderer(&BufferInfo(vertexBuf, vertexBufLen), nullptr, CUBE_GEOMETRY_SHADER);

	//_modelMat.SetRotation(glm::vec3(0, 90, 90));
}

ParticleRenderer::ParticleRenderer(BufferInfo* vertexBufInfo)
{
	_skipNumVertex = 50;
	_bBoxCenter = BufferTransformUtils::CalcCenter((float*)vertexBufInfo->buffer, vertexBufInfo->size / 4);

	_allParticlesRenderer = CreateAllParticlesRenderer(vertexBufInfo, nullptr, CUBE_GEOMETRY_SHADER);
	_fewParticlesRenderer = CreateFewParticlesRenderer(vertexBufInfo, nullptr, CUBE_GEOMETRY_SHADER);
}

ParticleRenderer::ParticleRenderer(BufferInfo* vertexBufInfo, BufferInfo* normalBufInfo)
{
	_skipNumVertex = 50;
	_bBoxCenter = BufferTransformUtils::CalcCenter((float*)vertexBufInfo->buffer, vertexBufInfo->size / 4);

	_allParticlesRenderer = CreateAllParticlesRenderer(vertexBufInfo, normalBufInfo, PHONG_CUBE_GEOMETRY_SHADER);
	_fewParticlesRenderer = CreateFewParticlesRenderer(vertexBufInfo, normalBufInfo, PHONG_CUBE_GEOMETRY_SHADER);
}

GLMeshRenderer* ParticleRenderer::CreateAllParticlesRenderer(BufferInfo* vertexBufInfo, BufferInfo* normalBufInfo, int shaderID)
{
	unsigned int colorBufLen = vertexBufInfo->size / 4;
	unsigned char* colorBuf = (unsigned char*)malloc(colorBufLen);
	memset(colorBuf, 255, colorBufLen);

	BaseModelIO modelIO;
	modelIO.SetVertexBufferInfo(vertexBufInfo);
	modelIO.SetColorBuffer((const char*)colorBuf, colorBufLen);
	if (normalBufInfo && normalBufInfo->HasData())
		modelIO.SetNormalBufferInfo(normalBufInfo);

	GLMeshRenderer* meshRenderer = new GLMeshRenderer(&modelIO, shaderID);
	meshRenderer->SetPrimitiveType(GLMeshRenderer::points);

	free(colorBuf);

	return meshRenderer;
}

GLMeshRenderer* ParticleRenderer::CreateFewParticlesRenderer(BufferInfo* highPolyVerBufInfo, BufferInfo* highPolyNormalBufInfo, int shaderID)
{
	BufferInfo lowPolyVerBufInfo = GenerateLowPolyVertexBuf(highPolyVerBufInfo);
	BufferInfo lowPolyColorBufInfo = GenerateLowPolyColorBuf(&lowPolyVerBufInfo);
	BufferInfo lowPolyNormalBufInfo;

	if (highPolyNormalBufInfo)
		lowPolyNormalBufInfo = GenerateLowPolyVertexBuf(highPolyNormalBufInfo);

	BaseModelIO modelIO;
	modelIO.SetVertexBufferInfo(&lowPolyVerBufInfo);
	modelIO.SetColorBufferInfo(&lowPolyColorBufInfo);
	modelIO.SetNormalBufferInfo(&lowPolyNormalBufInfo);

	GLMeshRenderer* meshRenderer = new GLMeshRenderer(&modelIO, shaderID);
	meshRenderer->SetPrimitiveType(GLMeshRenderer::points);

	free(lowPolyVerBufInfo.buffer);
	free(lowPolyColorBufInfo.buffer);
	free(lowPolyNormalBufInfo.buffer);

	return meshRenderer;
}

BufferInfo ParticleRenderer::GenerateLowPolyVertexBuf(BufferInfo* highPolyVerBufInfo)
{
	unsigned int bytesPVer = BYTES_PER_VERTEX;

	unsigned int highPolyVerCount = highPolyVerBufInfo->size / bytesPVer;
	unsigned int lowPolyVerCount = highPolyVerCount / (_skipNumVertex + 1);
	unsigned int lowPolyVerBufLen = lowPolyVerCount * bytesPVer;
	char* lowPolyVerBuf = (char*)malloc(lowPolyVerBufLen);

	unsigned int skipBytes = (_skipNumVertex + 1) * bytesPVer;
	unsigned int hIndex = 0; // high poly array index

	char* highPolyVerBuf = highPolyVerBufInfo->buffer;

	for (unsigned int i = 0; i < lowPolyVerBufLen; i += bytesPVer)
	{
		memcpy(&lowPolyVerBuf[i], &highPolyVerBuf[hIndex], bytesPVer);
		hIndex += skipBytes;
	}

	return BufferInfo(lowPolyVerBuf, lowPolyVerBufLen);
}

BufferInfo ParticleRenderer::GenerateLowPolyColorBuf(BufferInfo* lowPolyVerBufInfo)
{
	unsigned int lowPolyVerCount = lowPolyVerBufInfo->size / BYTES_PER_VERTEX;
	unsigned int lowPolyColorBufLen = lowPolyVerCount * BYTES_PER_COLOR;
	unsigned char* lowPolyColorBuf = (unsigned char*)malloc(lowPolyColorBufLen);
	memset(lowPolyColorBuf, 255, lowPolyColorBufLen);

	return BufferInfo((char*)lowPolyColorBuf, lowPolyColorBufLen);
}

void ParticleRenderer::UpdateColorBuffer(char* highPolyColorBuf, unsigned int highPolyColorBufLen)
{
	_allParticlesRenderer->UpdateColorBuffer(highPolyColorBuf, highPolyColorBufLen);

	unsigned int bytesPColor = BYTES_PER_COLOR;
	unsigned int highPolyColorCount = highPolyColorBufLen / bytesPColor;
	unsigned int lowPolyColorCount = highPolyColorCount / (_skipNumVertex + 1);
	unsigned int lowPolyColorBufLen = lowPolyColorCount * bytesPColor;
	unsigned char* lowPolyColorBuf = (unsigned char*)malloc(lowPolyColorBufLen);

	unsigned int skipBytes = (_skipNumVertex + 1) * bytesPColor;
	unsigned int hIndex = 0; // high poly array index

	for (unsigned int i = 0; i < lowPolyColorBufLen; i += bytesPColor)
	{
		memcpy(&lowPolyColorBuf[i], &highPolyColorBuf[hIndex], bytesPColor);
		hIndex += skipBytes;
	}

	if (_fewParticlesRenderer)
		_fewParticlesRenderer->UpdateColorBuffer(lowPolyColorBuf, lowPolyColorBufLen);

	free(lowPolyColorBuf);
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

void ParticleRenderer::SetPosition(float x, float y, float z)
{
	_modelMat.SetPos(x, y, z);
}

glm::vec3 ParticleRenderer::GetBBoxCenter()
{
	return _bBoxCenter;
}

void ParticleRenderer::DrawAllParticles()
{
	if(_allParticlesRenderer)
	{
		//_allParticlesRenderer->SetShader(PBR_CUBE_GEOMETRY_SHADER);
		_allParticlesRenderer->SetModelMatrix(_modelMat.m);
		_allParticlesRenderer->Draw();

		//GLMat mat1(_modelMat.m);
		//mat1.m[13] -= 1.0;

		//_allParticlesRenderer->SetShader(PHONG_CUBE_GEOMETRY_SHADER);
		//_allParticlesRenderer->SetModelMatrix(mat1.m);
		//_allParticlesRenderer->Draw();

		//GLMat mat2(_modelMat.m);
		//mat2.m[13] += 1.0;
		////mat2.m[14] += 1.5;

		//long startTime = Platform::GetTimeInMillis();
		//_allParticlesRenderer->SetShader(CUBE_GEOMETRY_SHADER);
		//_allParticlesRenderer->SetModelMatrix(_modelMat.m);
		//_allParticlesRenderer->Draw();
		//glFinish();
		//printf("\nTimeForCUBE_GEOMETRY_SHADER %ld", (Platform::GetTimeInMillis()-startTime));
	}
}

void ParticleRenderer::DrawFewParticles()
{
	if(_fewParticlesRenderer)
	{
		_fewParticlesRenderer->SetModelMatrix(_modelMat.m);
		_fewParticlesRenderer->Draw();
	}
	else
	{
		DrawAllParticles();
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
