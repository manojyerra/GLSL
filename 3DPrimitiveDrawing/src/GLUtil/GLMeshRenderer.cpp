#include "GLMeshRenderer.h"
#include "GLMemory.h"

GLMeshRenderer::GLMeshRenderer(ModelInfo* meshCreateInfo)
{
	_meshBuilder = new GLMeshBuilder();
	_meshBuilder->SetVertexBuffer(meshCreateInfo->GetVertexBuffer(), meshCreateInfo->GetVertexBufferSize());
	_meshBuilder->SetNormalBuffer(meshCreateInfo->GetNormalBuffer(), meshCreateInfo->GetNormalBufferSize());
	_meshBuilder->SetUVBuffer(meshCreateInfo->GetUVBuffer(), meshCreateInfo->GetUVBufferSize());
	_meshBuilder->SetColorBuffer(meshCreateInfo->GetColorBuffer(), meshCreateInfo->GetColorBufferSize());
	//_meshBuilder->SetIndicesBuffer(meshCreateInfo->GetIndicesBuffer(), meshCreateInfo->GetIndicesBufferSize());
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
	_shader = NULL;
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
			_shader = _basicShader;
		}
	}
	else if (_shaderType == COLOR_SHADER)
	{
		if (!_colorShader)
		{
			_colorShader = new ColorShader();
			_colorShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_colorShader->SetColorBufferID(_meshBuilder->GetColorBufferID());
			_shader = _colorShader;
		}
	}
	else if (_shaderType == PHONG_PER_VERTEX_SHADER)
	{
		if (!_phongPerVertexShader)
		{
			_phongPerVertexShader = new PhongShader(PhongShader::PER_VERTEX_SHADER);
			_phongPerVertexShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_phongPerVertexShader->SetNormalBufferID(_meshBuilder->GetNormalBufferID());
			_shader = _phongPerVertexShader;
		}
	}
	else if (_shaderType == PHONG_PER_PIXEL_SHADER)
	{
		if (!_phongPerPixelShader)
		{
			_phongPerPixelShader = new PhongShader(PhongShader::PER_PIXEL_SHADER);
			_phongPerPixelShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_phongPerPixelShader->SetNormalBufferID(_meshBuilder->GetNormalBufferID());
			_shader = _phongPerPixelShader;
		}
	}
	else if (_shaderType == PBR_SHADER)
	{
		if (!_pbrShader)
		{
			_pbrShader = new PBRShader();
			_pbrShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_pbrShader->SetNormalBufferID(_meshBuilder->GetNormalBufferID());
			_shader = _pbrShader;
		}
	}
}

void GLMeshRenderer::SetPrimitiveType(int primitiveType)
{
	_primitiveType = primitiveType;
}

void GLMeshRenderer::Draw()
{
	if (_shader)
	{
		_shader->SetModelMatrix(_modelMat.m);
		_shader->Begin();
		_shader->SetUniformsAndAttributes();
		glDrawArrays(_primitiveType, 0, _meshBuilder->GetVertexBufferSize()/12);
		_shader->End();
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
