#include "GLTexture.h"
#include "GLBatch.h"

GLTexture::GLTexture(const char* texturePath, float x, float y, float w, float h)
{
	_meshRenderer = NULL;

	_modelMat.m[0] = w;
	_modelMat.m[5] = h;
	_modelMat.m[12] = x;
	_modelMat.m[13] = y;

	GenerateGeometry(texturePath);
}

void GLTexture::SetBounds(float x, float y, float w, float h)
{
	_modelMat.m[0] = w;
	_modelMat.m[5] = h;

	_modelMat.m[12] = x;
	_modelMat.m[13] = y;
}

UVShader* GLTexture::GetShader()
{
	return (UVShader*)_meshRenderer->GetCurrentShader();
}

void GLTexture::Draw()
{
	_meshRenderer->SetModelMatrix(_modelMat.m);
	_meshRenderer->Draw();
}

void GLTexture::GenerateGeometry(const char* texturePath)
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

	ImageBuffer* imageBuffer = new ImageBuffer(texturePath);

	ModelInfo createInfo;
	createInfo.SetVertexBuffer(buffer->GetVertexBuffer(), buffer->GetVertexBufferSize());
	createInfo.SetUVBuffer(buffer->GetUVBuffer(), buffer->GetUVBufferSize());
	createInfo.SetImageBuffer(imageBuffer);

	_meshRenderer = new GLMeshRenderer(&createInfo, GLMeshRenderer::UV_SHADER);
	_meshRenderer->SetPrimitiveType(GLMeshRenderer::triangleStrip);

	delete buffer;
	delete imageBuffer;
}

GLTexture::~GLTexture()
{
	if (_meshRenderer)
	{
		delete _meshRenderer;
		_meshRenderer = NULL;
	}
}