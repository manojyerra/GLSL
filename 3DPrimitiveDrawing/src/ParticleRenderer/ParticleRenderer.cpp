#include "ParticleRenderer.h"
#include "Cam.h"
#include "BufferTransformUtils.h"
#include "Platform.h"
#include "GLState.h"

ParticleRenderer::ParticleRenderer(std::string filePath)
{
	_skipNumVertex = 50;
	_hasNormals = false;
	_particleLen = 0.00375;

	BinaryObjReader binaryObjReader(filePath);

	char* vertexBuf = (char*)binaryObjReader.GetVertexBuffer();
	unsigned int vertexBufLen = binaryObjReader.GetVertexBufferSize();

	_bBox = BufferTransformUtils::CalcAABB((float*)vertexBuf, vertexBufLen / 4);

	_allParticlesRenderer = CreateAllParticlesRenderer(&BufferInfo(vertexBuf, vertexBufLen), nullptr, CUBE_GEOMETRY_SHADER);
	_fewParticlesRenderer = CreateFewParticlesRenderer(&BufferInfo(vertexBuf, vertexBufLen), nullptr, CUBE_GEOMETRY_SHADER);
	SetParticleLen(_particleLen);
}

ParticleRenderer::ParticleRenderer(BufferInfo* vertexBufInfo)
{
	_skipNumVertex = 50;
	_hasNormals = false;
	_particleLen = 0.00375;

	_bBox = BufferTransformUtils::CalcAABB((float*)vertexBufInfo->buffer, vertexBufInfo->size / 4);

	glm::vec3 move(-_bBox.w / 2, _bBox.h / 2, _bBox.d / 2);
	BufferTransformUtils::Add((float*)vertexBufInfo->buffer, vertexBufInfo->size / 4, move);
	_bBox.Translate(move);

	_allParticlesRenderer = CreateAllParticlesRenderer(vertexBufInfo, nullptr, CUBE_GEOMETRY_SHADER);
	_fewParticlesRenderer = CreateFewParticlesRenderer(vertexBufInfo, nullptr, CUBE_GEOMETRY_SHADER);
	SetParticleLen(_particleLen);
}

ParticleRenderer::ParticleRenderer(BufferInfo* vertexBufInfo, BufferInfo* normalBufInfo)
{
	_skipNumVertex = 50;
	_hasNormals = true;
	_particleLen = 0.00375;

	_bBox = BufferTransformUtils::CalcAABB((float*)vertexBufInfo->buffer, vertexBufInfo->size / 4);

	glm::vec3 move(-_bBox.w / 2, _bBox.h / 2, _bBox.d / 2);
	BufferTransformUtils::Add((float*)vertexBufInfo->buffer, vertexBufInfo->size / 4, move);
	_bBox.Translate(move);

	_allParticlesRenderer = CreateAllParticlesRenderer(vertexBufInfo, normalBufInfo, PHONG_CUBE_GEOMETRY_SHADER);
	_fewParticlesRenderer = CreateFewParticlesRenderer(vertexBufInfo, normalBufInfo, PHONG_CUBE_GEOMETRY_SHADER);
	SetParticleLen(_particleLen);
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

void ParticleRenderer::SetParticleLen(float particleLen)
{
	_particleLen = particleLen;

	int shaderID = _allParticlesRenderer->GetCurrentShaderType();
	Shader* shader = _allParticlesRenderer->GetCurrentShader();

	if (shaderID == PHONG_CUBE_GEOMETRY_SHADER)
		((PhongCubeGeometryShader*)shader)->SetCubeHalfLen(_particleLen / 2.0f);
	else if (shaderID == CUBE_GEOMETRY_SHADER)
		((CubeGeometryShader*)shader)->SetCubeHalfLen(_particleLen);

	shaderID = _fewParticlesRenderer->GetCurrentShaderType();
	shader = _fewParticlesRenderer->GetCurrentShader();

	if (shaderID == PHONG_CUBE_GEOMETRY_SHADER)
		((PhongCubeGeometryShader*)shader)->SetCubeHalfLen(_particleLen / 2.0f);
	else if (shaderID == CUBE_GEOMETRY_SHADER)
		((CubeGeometryShader*)shader)->SetCubeHalfLen(_particleLen);
}

bool ParticleRenderer::SetDrawAs(int drawAs)
{
	bool retVal = false;

	if (drawAs == CUBES)
	{
		_allParticlesRenderer->SetShader(CUBE_GEOMETRY_SHADER);
		_fewParticlesRenderer->SetShader(CUBE_GEOMETRY_SHADER);
		SetParticleLen(_particleLen);
		retVal = true;
	}
	else if (drawAs == CUBES_WITH_LIGHTING)
	{
		if (_hasNormals)
		{
			_allParticlesRenderer->SetShader(PHONG_CUBE_GEOMETRY_SHADER);
			_fewParticlesRenderer->SetShader(PHONG_CUBE_GEOMETRY_SHADER);
			SetParticleLen(_particleLen);
			retVal = true;
		}
	}

	return retVal;
}

void ParticleRenderer::SetPosition(float x, float y, float z)
{
	_modelMat.SetPos(x, y, z);
	_allParticlesRenderer->SetModelMatrix(_modelMat.m);
	_fewParticlesRenderer->SetModelMatrix(_modelMat.m);
}

void ParticleRenderer::SetPosition(glm::vec3& pos)
{
	SetPosition(pos.x, pos.y, pos.z);
}

void ParticleRenderer::SetRotation(glm::vec3& rot)
{
	_modelMat.SetRotation(rot);
	_allParticlesRenderer->SetModelMatrix(_modelMat.m);
	_fewParticlesRenderer->SetModelMatrix(_modelMat.m);
}

GLMat ParticleRenderer::GetModelMat()
{
	return _modelMat;
}

AABB ParticleRenderer::GetBBox()
{
	return _bBox;
}

glm::vec3 ParticleRenderer::GetBBoxCenter()
{
	return _bBox.Center();
}

glm::vec3 ParticleRenderer::GetBBoxCenterAfterTransform()
{
	glm::vec4 center = glm::vec4(_bBox.Center(), 1.0f);
	glm::mat4 mat = glm::make_mat4(_modelMat.m);
	glm::vec3 result = glm::vec3(mat * center);
	return result;
}

void ParticleRenderer::DrawForPicking()
{
	unsigned int shaderType = _allParticlesRenderer->GetCurrentShaderType();
	_allParticlesRenderer->SetShader(CUBE_GEOMETRY_SHADER_FOR_PICKING);
	_allParticlesRenderer->Draw();
	_allParticlesRenderer->SetShader(shaderType);
}

void ParticleRenderer::DrawAllParticles()
{
	if(_allParticlesRenderer)
	{
		bool cullEnable = GLState::GLEnable(GL_CULL_FACE, false);
		_allParticlesRenderer->Draw();
		GLState::GLEnable(GL_CULL_FACE, cullEnable);
	}
}

void ParticleRenderer::DrawFewParticles()
{
	if(_fewParticlesRenderer)
	{
		bool cullEnable = GLState::GLEnable(GL_CULL_FACE, false);
		_fewParticlesRenderer->Draw();
		GLState::GLEnable(GL_CULL_FACE, cullEnable);
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
