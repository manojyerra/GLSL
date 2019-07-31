#include "GLMeshRenderer.h"
#include "GLMemory.h"

GLMeshRenderer::GLMeshRenderer(ModelInfo* modelInfo, int shaderType)
{
	_bufferBuilder = new GLBufferBuilder();
	_bufferBuilder->SetVertexBuffer(modelInfo->GetVertexBuffer(), modelInfo->GetVertexBufferSize());
	_bufferBuilder->SetNormalBuffer(modelInfo->GetNormalBuffer(), modelInfo->GetNormalBufferSize());
	_bufferBuilder->SetUVBuffer(modelInfo->GetUVBuffer(), modelInfo->GetUVBufferSize());
	_bufferBuilder->SetColorBuffer(modelInfo->GetColorBuffer(), modelInfo->GetColorBufferSize());
	_bufferBuilder->SetIndicesBuffer(modelInfo->GetIndicesBuffer(), modelInfo->GetIndicesBufferSize());
	_bufferBuilder->SetImageBuffer(modelInfo->GetImageBuffer());
	_bufferBuilder->build();

	_bBox = modelInfo->GetAABB();
	_bBox.print();

	_primitiveType = triangles;

	_basicShader = NULL;
	_colorShader = NULL;
	_phongPerVertexShader = NULL;
	_phongPerPixelShader = NULL;
	_phongWithTexPerVertexShader = NULL;
	_pbrShader = NULL;
	_pbrWithTextureShader = NULL;
	_cubeGeometryShader = NULL;
	_ssaoGeometryPassShader = NULL;
	_ssaoShader = NULL;
	_ssaoBlurShader = NULL;
	_ssaoMapOverlayShader = NULL;

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
			_basicShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
		}

		_shader = _basicShader;
	}
	else if (_shaderType == COLOR_SHADER)
	{
		if (!_colorShader)
		{
			_colorShader = new ColorShader();
			_colorShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_colorShader->SetColorBufferID(_bufferBuilder->GetColorBufferID());
		}

		_shader = _colorShader;
	}
	else if (_shaderType == UV_SHADER)
	{
		if (!_uvShader)
		{
			_uvShader = new UVShader();
			_uvShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_uvShader->SetUVBufferID(_bufferBuilder->GetUVBufferID());
			_uvShader->SetTextureID(_bufferBuilder->GetBaseTexID());
		}

		_shader = _uvShader;
	}
	else if (_shaderType == PHONG_PER_VERTEX_SHADER)
	{
		if (!_phongPerVertexShader)
		{
			_phongPerVertexShader = new PhongShader(PhongShader::PER_VERTEX_SHADER);
			_phongPerVertexShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_phongPerVertexShader->SetNormalBufferID(_bufferBuilder->GetNormalBufferID());
		}

		_shader = _phongPerVertexShader;
	}
	else if (_shaderType == PHONG_PER_PIXEL_SHADER)
	{
		if (!_phongPerPixelShader)
		{
			_phongPerPixelShader = new PhongShader(PhongShader::PER_PIXEL_SHADER);
			_phongPerPixelShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_phongPerPixelShader->SetNormalBufferID(_bufferBuilder->GetNormalBufferID());
		}

		_shader = _phongPerPixelShader;
	}
	else if (_shaderType == PHONG_WITH_TEXTURE_PER_VERTEX_SHADER)
	{
		if (!_phongWithTexPerVertexShader)
		{
			_phongWithTexPerVertexShader = new PhongWithTextureShader(PhongWithTextureShader::PER_VERTEX_SHADER);
			_phongWithTexPerVertexShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_phongWithTexPerVertexShader->SetNormalBufferID(_bufferBuilder->GetNormalBufferID());
			_phongWithTexPerVertexShader->SetUVBufferID(_bufferBuilder->GetUVBufferID());
			_phongWithTexPerVertexShader->SetBaseTexID(_bufferBuilder->GetBaseTexID());
		}

		_shader = _phongWithTexPerVertexShader;
	}
	else if (_shaderType == PHONG_WITH_TEXTURE_PER_PIXEL_SHADER)
	{
		if (!_phongWithTexPerPixelShader)
		{
			_phongWithTexPerPixelShader = new PhongWithTextureShader(PhongWithTextureShader::PER_PIXEL_SHADER);
			_phongWithTexPerPixelShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_phongWithTexPerPixelShader->SetNormalBufferID(_bufferBuilder->GetNormalBufferID());
			_phongWithTexPerPixelShader->SetUVBufferID(_bufferBuilder->GetUVBufferID());
			_phongWithTexPerPixelShader->SetBaseTexID(_bufferBuilder->GetBaseTexID());
		}

		_shader = _phongWithTexPerPixelShader;
	}
	else if (_shaderType == PBR_SHADER)
	{
		if (!_pbrShader)
		{
			_pbrShader = new PBRShader();
			_pbrShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_pbrShader->SetNormalBufferID(_bufferBuilder->GetNormalBufferID());
		}

		_shader = _pbrShader;
	}
	else if (_shaderType == PBR_WITH_TEXTURE_SHADER)
	{
		if (!_pbrWithTextureShader)
		{
			_pbrWithTextureShader = new PBRWithTextureShader();
			_pbrWithTextureShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_pbrWithTextureShader->SetNormalBufferID(_bufferBuilder->GetNormalBufferID());
			_pbrWithTextureShader->SetUVBufferID(_bufferBuilder->GetUVBufferID());
			_pbrWithTextureShader->SetBaseTexID(_bufferBuilder->GetBaseTexID());
		}

		_shader = _pbrWithTextureShader;
	}
	else if(_shaderType == CUBE_GEOMETRY_SHADER)
	{
		if(!_cubeGeometryShader)
		{
			_cubeGeometryShader = new CubeGeometryShader();
			_cubeGeometryShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_cubeGeometryShader->SetColorBufferID(_bufferBuilder->GetColorBufferID());
		}

		_shader = _cubeGeometryShader;
	}
	else if(_shaderType == SSAO_GEOMETRY_PASS_SHADER)
	{
		if(!_ssaoGeometryPassShader)
		{
			_ssaoGeometryPassShader = new SSAOGeometryPassShader();
			_ssaoGeometryPassShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_ssaoGeometryPassShader->SetNormalBufferID(_bufferBuilder->GetNormalBufferID());
		}

		_shader = _ssaoGeometryPassShader;
	}
	else if (_shaderType == SSAO_SHADER)
	{
		if (!_ssaoShader)
		{
			_ssaoShader = new SSAOShader();
			_ssaoShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_ssaoShader->SetUVBufferID(_bufferBuilder->GetUVBufferID());
		}

		_shader = _ssaoShader;
	}
	else if (_shaderType == SSAO_BLUR_SHADER)
	{
		if (!_ssaoBlurShader)
		{
			_ssaoBlurShader = new SSAOBlurShader();
			_ssaoBlurShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_ssaoBlurShader->SetUVBufferID(_bufferBuilder->GetUVBufferID());
		}

		_shader = _ssaoBlurShader;
	}
	else if(_shaderType == SSAO_MAP_OVERLAY_SHADER)
	{
		if (!_ssaoMapOverlayShader)
		{
			_ssaoMapOverlayShader = new SSAOMapOverlayShader();
			_ssaoMapOverlayShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_ssaoMapOverlayShader->SetUVBufferID(_bufferBuilder->GetUVBufferID());
		}

		_shader = _ssaoMapOverlayShader;
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
	else if (_shaderType == CUBE_GEOMETRY_SHADER)
	{
		return _cubeGeometryShader;
	}
	else if (_shaderType == SSAO_GEOMETRY_PASS_SHADER)
	{
		return _ssaoGeometryPassShader;
	}
	else if (_shaderType == SSAO_SHADER)
	{
		return _ssaoShader;
	}
	else if (_shaderType == SSAO_BLUR_SHADER)
	{
		return _ssaoBlurShader;
	}
	else if (_shaderType == SSAO_MAP_OVERLAY_SHADER)
	{
		return _ssaoMapOverlayShader;
	}

	return NULL;
}

Shader* GLMeshRenderer::GetCurrentShader()
{
	return _shader;
}

unsigned int GLMeshRenderer::GetCurrentShaderType()
{
	return _shaderType;
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
		//SetShader(BASIC_SHADER);

		_shader->SetModelMatrix(_modelMat.m);
		_shader->Begin();

		//_basicShader->SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
		//_shader->SetUniformsAndAttributes();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glDrawArrays(_primitiveType, 0, (GLsizei)(_bufferBuilder->GetVertexBufferSize() / 12));

		//_basicShader->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
		_shader->SetUniformsAndAttributes();
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(_primitiveType, 0, (GLsizei)(_bufferBuilder->GetVertexBufferSize() / 12));

		_shader->End();
	}
}

GLMeshRenderer::~GLMeshRenderer()
{
	if (_bufferBuilder)
	{
		delete _bufferBuilder;
		_bufferBuilder = NULL;
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

	if(_cubeGeometryShader)
	{
		delete _cubeGeometryShader;
		_cubeGeometryShader = NULL;
	}

	if(_ssaoGeometryPassShader)
	{
		delete _ssaoGeometryPassShader;
		_ssaoGeometryPassShader = NULL;
	}

	if (_ssaoShader)
	{
		delete _ssaoShader;
		_ssaoShader = NULL;
	}

	if (_ssaoBlurShader)
	{
		delete _ssaoBlurShader;
		_ssaoBlurShader = NULL;
	}

	if(_ssaoMapOverlayShader)
	{
		delete _ssaoMapOverlayShader;
		_ssaoMapOverlayShader = NULL;
	}
}