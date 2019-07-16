#include "GLMeshRenderer.h"
#include "GLMemory.h"


GLMeshRenderer::GLMeshRenderer(ObjReader* reader)
{
	_meshBuilder = new GLMeshBuilder();
	_meshBuilder->SetVertexBuffer(reader->GetVertexBuffer(), reader->GetVertexBufferSize());
	_meshBuilder->SetNormalBuffer(reader->GetNormalBuffer(), reader->GetNoralBufferSize());
	_meshBuilder->SetUVBuffer(reader->GetUVBuffer(), reader->GetUVBufferSize());
	_meshBuilder->SetImageBuffer(reader->GetImageBuffer());
	_meshBuilder->build();

	CommonInit();
}

GLMeshRenderer::GLMeshRenderer(BinaryObjReader* reader)
{
	_meshBuilder = new GLMeshBuilder();
	_meshBuilder->SetVertexBuffer(reader->GetVertexBuffer(), reader->GetVertexBufferSize());
	_meshBuilder->SetNormalBuffer(reader->GetNormalBuffer(), reader->GetNoralBufferSize());
	_meshBuilder->SetUVBuffer(reader->GetUVBuffer(), reader->GetUVBufferSize());
	_meshBuilder->SetImageBuffer(reader->GetImageBuffer());
	_meshBuilder->build();

	CommonInit();
}

GLMeshRenderer::GLMeshRenderer(GLMeshCreateInfo* meshCreateInfo)
{
	_meshBuilder = new GLMeshBuilder();
	_meshBuilder->SetVertexBuffer(meshCreateInfo->GetVertexBuffer(), meshCreateInfo->GetVertexBufferSize());
	_meshBuilder->SetNormalBuffer(meshCreateInfo->GetNormalBuffer(), meshCreateInfo->GetNormalBufferSize());
	_meshBuilder->SetUVBuffer(meshCreateInfo->GetUVBuffer(), meshCreateInfo->GetUVBufferSize());
	_meshBuilder->SetColorBuffer(meshCreateInfo->GetColorBuffer(), meshCreateInfo->GetColorBufferSize());
	_meshBuilder->SetImageBuffer(meshCreateInfo->GetImageBuffer());
	_meshBuilder->build();

	CommonInit();
}

void GLMeshRenderer::CommonInit()
{
	_basicShader = NULL;
	_colorShader = NULL;
	_phongPerVertexShader = NULL;
	_phongPerPixelShader = NULL;
	_pbrShader = NULL;
	_primitiveType = triangles;

	_shaderType = BASIC_SHADER;

	SetShader(_shaderType);
}

void GLMeshRenderer::SetModelMatrix(float* mat)
{
	_modelMat.Copy(mat);
}

void GLMeshRenderer::SetPos(float x, float y, float z)
{
	_modelMat.SetPos(x, y, z);
}

void GLMeshRenderer::SetShader(int shaderType)
{
	_shaderType = shaderType;

	if (_shaderType == BASIC_SHADER)
	{
		if (!_basicShader)
		{
			_basicShader = new BasicShader();
			_basicShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
		}
	}
	else if (_shaderType == COLOR_SHADER)
	{
		if (!_colorShader)
		{
			_colorShader = new ColorShader();
			_colorShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_colorShader->SetColorBufferID(_meshBuilder->GetColorBufferID());
		}
	}
	else if (_shaderType == PHONG_PER_VERTEX_SHADER)
	{
		if (!_phongPerVertexShader)
		{
			_phongPerVertexShader = new PhongShader(PhongShader::PER_VERTEX_SHADER);
			_phongPerVertexShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_phongPerVertexShader->SetNormalBufferID(_meshBuilder->GetNormalBufferID());
		}
	}
	else if (_shaderType == PHONG_PER_PIXEL_SHADER)
	{
		if (!_phongPerPixelShader)
		{
			_phongPerPixelShader = new PhongShader(PhongShader::PER_PIXEL_SHADER);
			_phongPerPixelShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_phongPerPixelShader->SetNormalBufferID(_meshBuilder->GetNormalBufferID());
		}
	}
	else if (_shaderType == PBR_SHADER)
	{
		if (!_pbrShader)
		{
			_pbrShader = new PBRShader();
			_pbrShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_pbrShader->SetNormalBufferID(_meshBuilder->GetNormalBufferID());
		}
	}
}

void GLMeshRenderer::SetPrimitiveType(int primitiveType)
{
	_primitiveType = primitiveType;
}

void GLMeshRenderer::Draw()
{
	if (_shaderType == BASIC_SHADER)
	{
		_basicShader->SetModelMatrix(_modelMat.m);
		_basicShader->Begin();
		_basicShader->SetUniformsAndAttributes();
		glDrawArrays(_primitiveType, 0, _meshBuilder->GetVertexBufferSize()/12);
		_basicShader->End();
	}
	if (_shaderType == COLOR_SHADER)
	{
		_colorShader->SetModelMatrix(_modelMat.m);
		_colorShader->Begin();
		_colorShader->SetUniformsAndAttributes();
		glDrawArrays(_primitiveType, 0, _meshBuilder->GetVertexBufferSize() / 12);
		_colorShader->End();
	}
	else if (_shaderType == PHONG_PER_VERTEX_SHADER)
	{
		_phongPerVertexShader->SetModelMatrix(_modelMat.m);
		_phongPerVertexShader->Begin();
		_phongPerVertexShader->SetUniformsAndAttributes();
		glDrawArrays(_primitiveType, 0, _meshBuilder->GetVertexBufferSize() / 12);
		_phongPerVertexShader->End();
	}
	else if (_shaderType == PHONG_PER_PIXEL_SHADER)
	{
		_phongPerPixelShader->SetModelMatrix(_modelMat.m);
		_phongPerPixelShader->Begin();
		_phongPerPixelShader->SetUniformsAndAttributes();
		glDrawArrays(_primitiveType, 0, _meshBuilder->GetVertexBufferSize() / 12);
		_phongPerPixelShader->End();
	}
	else if (_shaderType == PBR_SHADER)
	{
		_pbrShader->SetModelMatrix(_modelMat.m);
		_pbrShader->Begin();
		_pbrShader->SetUniformsAndAttributes();
		glDrawArrays(_primitiveType, 0, _meshBuilder->GetVertexBufferSize() / 12);
		_pbrShader->End();
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

	if (_colorShader)
	{
		delete _colorShader;
		_colorShader = NULL;
	}

	if (_phongPerVertexShader)
	{
		delete _phongPerVertexShader;
		_phongPerVertexShader = NULL;
	}

	if (_phongPerPixelShader)
	{
		delete _phongPerPixelShader;
		_phongPerPixelShader = NULL;
	}

	if (_pbrShader)
	{
		delete _pbrShader;
		_pbrShader = NULL;
	}
}
