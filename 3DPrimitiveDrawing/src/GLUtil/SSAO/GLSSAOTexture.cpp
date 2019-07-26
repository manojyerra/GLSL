#include "GLSSAOTexture.h"
#include "GLBatch.h"

GLSSAOTexture::GLSSAOTexture(float x, float y, float w, float h)
{
	_meshRenderer = nullptr;

	_modelMat.m[0] = w;
	_modelMat.m[5] = h;
	_modelMat.m[12] = x;
	_modelMat.m[13] = y;

	GenerateGeometry();
}

void GLSSAOTexture::SetBounds(float x, float y, float w, float h)
{
	_modelMat.m[0] = w;
	_modelMat.m[5] = h;

	_modelMat.m[12] = x;
	_modelMat.m[13] = y;
}

void GLSSAOTexture::GenerateGeometry()
{
	GLBatch* buffer = new GLBatch(100, false, true, false);

	buffer->glBegin();

	buffer->glTexCoord2f(0, 1);
	buffer->glVertex3f(0, 0, 0);

	buffer->glTexCoord2f(1, 1);
	buffer->glVertex3f(1, 0, 0);

	buffer->glTexCoord2f(0, 0);
	buffer->glVertex3f(0, 1, 0);

	buffer->glTexCoord2f(1, 0);
	buffer->glVertex3f(1, 1, 0);

	ModelInfo createInfo;
	createInfo.SetVertexBuffer(buffer->GetVertexBuffer(), buffer->GetVertexBufferSize());
	createInfo.SetUVBuffer(buffer->GetUVBuffer(), buffer->GetUVBufferSize());

	_meshRenderer = new GLMeshRenderer(&createInfo, GLMeshRenderer::SSAO_SHADER);
	_meshRenderer->SetPrimitiveType(GLMeshRenderer::triangleStrip);

	delete buffer;
}

void GLSSAOTexture::SetGPositionTexID(unsigned int texID)
{
	SSAOShader* ssaoShader = (SSAOShader*)_meshRenderer->GetCurrentShader();
	ssaoShader->SetGPositionTexID(texID);
}

void GLSSAOTexture::SetGNormalTexID(unsigned int texID)
{
	SSAOShader* ssaoShader = (SSAOShader*)_meshRenderer->GetCurrentShader();
	ssaoShader->SetGNormalTexID(texID);
}

void GLSSAOTexture::SetNoiseTexID(unsigned int texID)
{
	SSAOShader* ssaoShader = (SSAOShader*)_meshRenderer->GetCurrentShader();
	ssaoShader->SetNoiseTexID(texID);
}

void GLSSAOTexture::SetSamples(std::vector<glm::vec3> samples)
{
	SSAOShader* ssaoShader = (SSAOShader*)_meshRenderer->GetCurrentShader();
	ssaoShader->SetSamples(samples);
}

void GLSSAOTexture::Draw()
{
	_meshRenderer->SetModelMatrix(_modelMat.m);
	_meshRenderer->Draw();
}

GLSSAOTexture::~GLSSAOTexture()
{
	if (_meshRenderer)
	{
		delete _meshRenderer;
		_meshRenderer = NULL;
	}
}
