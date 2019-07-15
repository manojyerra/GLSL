#include "GLMeshRenderer.h"
#include "GLMemory.h"


GLMeshRenderer::GLMeshRenderer(ObjReader* reader)
{
	_meshBuilder = new GLMeshBuilder();
	_meshBuilder->SetVertexBuffer(reader->GetVertexBuffer(), reader->GetVertexBufferSize());
	_meshBuilder->SetUVBuffer(reader->GetUVBuffer(), reader->GetUVBufferSize());
	_meshBuilder->SetNormalBuffer(reader->GetNormalBuffer(), reader->GetNoralBufferSize());
	_meshBuilder->SetImageBuffer(reader->GetImageBuffer());
	_meshBuilder->build();

	CommonInit();
}

GLMeshRenderer::GLMeshRenderer(BinaryObjReader* reader)
{
	_meshBuilder = new GLMeshBuilder();
	_meshBuilder->SetVertexBuffer(reader->GetVertexBuffer(), reader->GetVertexBufferSize());
	_meshBuilder->SetUVBuffer(reader->GetUVBuffer(), reader->GetUVBufferSize());
	_meshBuilder->SetNormalBuffer(reader->GetNormalBuffer(), reader->GetNoralBufferSize());
	_meshBuilder->SetImageBuffer(reader->GetImageBuffer());
	_meshBuilder->build();

	CommonInit();
}

void GLMeshRenderer::CommonInit()
{
	_basicShader = NULL;
	_shaderType = BASIC_SHADER;

	SetShader(_shaderType);
}

void GLMeshRenderer::SetShader(int shaderType)
{
	_shaderType = shaderType;

	if (_shaderType == BASIC_SHADER)
	{
		_basicShader = new BasicShader();
		_basicShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
	}
}

void GLMeshRenderer::Draw()
{
	if (_shaderType == BASIC_SHADER)
	{
		_basicShader->Begin();
		_basicShader->SetUniformsAndAttributes();
		glDrawArrays(GL_TRIANGLES, 0, _meshBuilder->GetVertexBufferSize()/12);
		_basicShader->End();
	}
}

GLMeshRenderer::~GLMeshRenderer()
{
	if (_meshBuilder)
	{
		delete _meshBuilder;
		_meshBuilder = NULL;
	}

	if (_basicShader)
	{
		delete _basicShader;
		_basicShader = NULL;
	}
}
