#include "GLMeshRenderer.h"
#include "GLMemory.h"

GLMeshRenderer::GLMeshRenderer(ModelInfo* meshCreateInfo, int shaderType)
{
	_meshBuilder = new GLMeshBuilder();
	_meshBuilder->SetVertexBuffer(meshCreateInfo->GetVertexBuffer(), meshCreateInfo->GetVertexBufferSize());
	_meshBuilder->SetNormalBuffer(meshCreateInfo->GetNormalBuffer(), meshCreateInfo->GetNormalBufferSize());
	_meshBuilder->SetUVBuffer(meshCreateInfo->GetUVBuffer(), meshCreateInfo->GetUVBufferSize());
	_meshBuilder->SetColorBuffer(meshCreateInfo->GetColorBuffer(), meshCreateInfo->GetColorBufferSize());
	_meshBuilder->SetIndicesBuffer(meshCreateInfo->GetIndicesBuffer(), meshCreateInfo->GetIndicesBufferSize());
	_meshBuilder->SetImageBuffer(meshCreateInfo->GetImageBuffer());
	_meshBuilder->build();

	_primitiveType = triangles;

	_basicShader = NULL;
	_colorShader = NULL;
	_phongPerVertexShader = NULL;
	_phongPerPixelShader = NULL;
	_phongWithTexPerVertexShader = NULL;
	_pbrShader = NULL;
	_pbrWithTextureShader = NULL;
	_shader = NULL;

	_shaderType = shaderType;

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

glm::vec3 GLMeshRenderer::GetPos()
{
	return glm::vec3(_modelMat.m[12], _modelMat.m[13], _modelMat.m[14]);
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

		_shader = _basicShader;
	}
	else if (_shaderType == COLOR_SHADER)
	{
		if (!_colorShader)
		{
			_colorShader = new ColorShader();
			_colorShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_colorShader->SetColorBufferID(_meshBuilder->GetColorBufferID());
		}

		_shader = _colorShader;
	}
	else if (_shaderType == UV_SHADER)
	{
		if (!_uvShader)
		{
			_uvShader = new UVShader();
			_uvShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_uvShader->SetUVBufferID(_meshBuilder->GetUVBufferID());
			_uvShader->SetTextureID(_meshBuilder->GetBaseTexID());
		}

		_shader = _uvShader;
	}
	else if (_shaderType == PHONG_PER_VERTEX_SHADER)
	{
		if (!_phongPerVertexShader)
		{
			_phongPerVertexShader = new PhongShader(PhongShader::PER_VERTEX_SHADER);
			_phongPerVertexShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_phongPerVertexShader->SetNormalBufferID(_meshBuilder->GetNormalBufferID());
		}

		_shader = _phongPerVertexShader;
	}
	else if (_shaderType == PHONG_PER_PIXEL_SHADER)
	{
		if (!_phongPerPixelShader)
		{
			_phongPerPixelShader = new PhongShader(PhongShader::PER_PIXEL_SHADER);
			_phongPerPixelShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_phongPerPixelShader->SetNormalBufferID(_meshBuilder->GetNormalBufferID());
		}

		_shader = _phongPerPixelShader;
	}
	else if (_shaderType == PHONG_WITH_TEXTURE_PER_VERTEX_SHADER)
	{
		if (!_phongWithTexPerVertexShader)
		{
			_phongWithTexPerVertexShader = new PhongWithTextureShader(PhongWithTextureShader::PER_VERTEX_SHADER);
			_phongWithTexPerVertexShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_phongWithTexPerVertexShader->SetNormalBufferID(_meshBuilder->GetNormalBufferID());
			_phongWithTexPerVertexShader->SetUVBufferID(_meshBuilder->GetUVBufferID());
			_phongWithTexPerVertexShader->SetBaseTexID(_meshBuilder->GetBaseTexID());
		}

		_shader = _phongWithTexPerVertexShader;
	}
	else if (_shaderType == PHONG_WITH_TEXTURE_PER_PIXEL_SHADER)
	{
		if (!_phongWithTexPerPixelShader)
		{
			_phongWithTexPerPixelShader = new PhongWithTextureShader(PhongWithTextureShader::PER_PIXEL_SHADER);
			_phongWithTexPerPixelShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_phongWithTexPerPixelShader->SetNormalBufferID(_meshBuilder->GetNormalBufferID());
			_phongWithTexPerPixelShader->SetUVBufferID(_meshBuilder->GetUVBufferID());
			_phongWithTexPerPixelShader->SetBaseTexID(_meshBuilder->GetBaseTexID());
		}

		_shader = _phongWithTexPerPixelShader;
	}
	else if (_shaderType == PBR_SHADER)
	{
		if (!_pbrShader)
		{
			_pbrShader = new PBRShader();
			_pbrShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_pbrShader->SetNormalBufferID(_meshBuilder->GetNormalBufferID());
		}

		_shader = _pbrShader;
	}
	else if (_shaderType == PBR_WITH_TEXTURE_SHADER)
	{
		if (!_pbrWithTextureShader)
		{
			_pbrWithTextureShader = new PBRWithTextureShader();
			_pbrWithTextureShader->SetVertexBufferID(_meshBuilder->GetVertexBufferID());
			_pbrWithTextureShader->SetNormalBufferID(_meshBuilder->GetNormalBufferID());
			_pbrWithTextureShader->SetUVBufferID(_meshBuilder->GetUVBufferID());
			_pbrWithTextureShader->SetBaseTexID(_meshBuilder->GetBaseTexID());
		}

		_shader = _pbrWithTextureShader;
	}
}

Shader* GLMeshRenderer::GetShader(int shaderType)
{
	if (_shaderType == BASIC_SHADER)
	{
		return _basicShader;
	}
	else if (_shaderType == COLOR_SHADER)
	{
		return _colorShader;
	}
	else if (_shaderType == UV_SHADER)
	{
		return _uvShader;
	}
	else if (_shaderType == PHONG_PER_VERTEX_SHADER)
	{
		return _phongPerVertexShader;
	}
	else if (_shaderType == PHONG_PER_PIXEL_SHADER)
	{
		return _phongPerPixelShader;
	}
	else if (_shaderType == PBR_SHADER)
	{
		return _pbrShader;
	}
	else if (_shaderType == PBR_WITH_TEXTURE_SHADER)
	{
		return _pbrWithTextureShader;
	}

	return NULL;
}

Shader* GLMeshRenderer::GetCurrentShader()
{
	return _shader;
}

void GLMeshRenderer::SetPrimitiveType(int primitiveType)
{
	_primitiveType = primitiveType;
}

void GLMeshRenderer::SetAlpha(float alpha)
{
	_shader->SetAlpha(alpha);
}

void GLMeshRenderer::Draw()
{
	if (_shader)
	{
		_shader->SetModelMatrix(_modelMat.m);
		_shader->Begin();
		_shader->SetUniformsAndAttributes();
		glDrawArrays(_primitiveType, 0, _meshBuilder->GetVertexBufferSize() / 12);
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

	if (_uvShader)
	{
		delete _uvShader;
		_uvShader = NULL;
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

	if (_phongWithTexPerVertexShader)
	{
		delete _phongWithTexPerVertexShader;
		_phongWithTexPerVertexShader = NULL;
	}

	if (_phongWithTexPerPixelShader)
	{
		delete _phongWithTexPerPixelShader;
		_phongWithTexPerPixelShader = NULL;
	}

	if (_pbrShader)
	{
		delete _pbrShader;
		_pbrShader = NULL;
	}

	if (_pbrWithTextureShader)
	{
		delete _pbrWithTextureShader;
		_pbrWithTextureShader = NULL;
	}
}