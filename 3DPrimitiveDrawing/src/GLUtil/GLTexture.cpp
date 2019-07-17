#include "GLTexture.h"
#include "GLBuffer.h"

GLTexture::GLTexture(float x, float y, float w, float h)
{
	_meshRenderer = NULL;

	_modelMat.m[0] = w;
	_modelMat.m[5] = h;
	_modelMat.m[12] = x;
	_modelMat.m[13] = y;

	GenerateGeometry();

	//ImageBuffer* imgBuf = new ImageBuffer("data/Sample.png");
}

void GLTexture::SetBounds(float x, float y, float w, float h)
{
	_modelMat.m[0] = w;
	_modelMat.m[5] = h;

	_modelMat.m[12] = x;
	_modelMat.m[13] = y;
}

void GLTexture::Draw()
{
	_meshRenderer->SetModelMatrix(_modelMat.m);
	_meshRenderer->Draw();
}

void GLTexture::GenerateGeometry()
{
	GLBuffer* buffer = new GLBuffer(100, true, true, false);

	buffer->glBegin(GL_TRIANGLE_STRIP);

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
	//createInfo.SetColorBuffer(buffer->GetColorBuffer(), buffer->GetColorBufferSize());

	_meshRenderer = new GLMeshRenderer(&createInfo);
	_meshRenderer->SetShader(GLMeshRenderer::BASIC_SHADER);
	_meshRenderer->SetPrimitiveType(GLMeshRenderer::triangleStrip);

	delete buffer;
}

GLTexture::~GLTexture()
{
	if (_meshRenderer)
	{
		delete _meshRenderer;
		_meshRenderer = NULL;
	}
}