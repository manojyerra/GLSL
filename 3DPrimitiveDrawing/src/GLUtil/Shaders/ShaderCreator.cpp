#include "ShaderCreator.h"

ShaderCreator::ShaderCreator(unsigned int shaderType, GLBufferBuilder* bufferBuilder)
{
	_basicShader = NULL;
	_colorShader = NULL;
	_uvShader = NULL;
	_cubeGeometryShader = NULL;
	_quadGeometryShader = NULL;
	_sphereGeometryShader = NULL;

	_phongPerVertexShader = NULL;
	_phongPerPixelShader = NULL;
	_phongColorShader = NULL;
	_phongWithTexPerVertexShader = NULL;
	_phongWithTexPerPixelShader = NULL;

	_pbrShader = NULL;
	_pbrColorShader = NULL;
	_pbrWithTextureShader = NULL;

	_ssaoShader = NULL;
	_ssaoBlurShader = NULL;
	_ssaoMapOverlayShader = NULL;
	_ssaoGeometryPassShader = NULL;

	_shader = NULL;

	_shaderType = shaderType;
	_bufferBuilder = bufferBuilder;

	SetShader(_shaderType);
}

void ShaderCreator::SetShader(int shaderType)
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
	else if (_shaderType == PHONG_COLOR_SHADER)
	{
		if (!_phongColorShader)
		{
			_phongColorShader = new PhongColorShader();
			_phongColorShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_phongColorShader->SetColorBufferID(_bufferBuilder->GetColorBufferID());
			_phongColorShader->SetNormalBufferID(_bufferBuilder->GetNormalBufferID());
		}

		_shader = _phongColorShader;
	}
	else if (_shaderType == PHONG_WITH_TEXTURE_PER_VERTEX_SHADER)
	{
		if (!_phongWithTexPerVertexShader)
		{
			_phongWithTexPerVertexShader = new PhongWithTextureShader(PhongWithTextureShader::PER_VERTEX_SHADER);
			_phongWithTexPerVertexShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_phongWithTexPerVertexShader->SetNormalBufferID(_bufferBuilder->GetNormalBufferID());
			_phongWithTexPerVertexShader->SetUVBufferID(_bufferBuilder->GetUVBufferID());
			_phongWithTexPerVertexShader->SetTextureID(_bufferBuilder->GetBaseTexID());
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
			_phongWithTexPerPixelShader->SetTextureID(_bufferBuilder->GetBaseTexID());
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
			_pbrWithTextureShader->SetTextureID(_bufferBuilder->GetBaseTexID());
		}

		_shader = _pbrWithTextureShader;
	}
	else if (_shaderType == PBR_COLOR_SHADER)
	{
		if (!_pbrColorShader)
		{
			_pbrColorShader = new PBRColorShader();
			_pbrColorShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_pbrColorShader->SetNormalBufferID(_bufferBuilder->GetNormalBufferID());
			_pbrColorShader->SetColorBufferID(_bufferBuilder->GetColorBufferID());
		}

		_shader = _pbrColorShader;
	}
	else if (_shaderType == CUBE_GEOMETRY_SHADER)
	{
		if (!_cubeGeometryShader)
		{
			_cubeGeometryShader = new CubeGeometryShader();
			_cubeGeometryShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_cubeGeometryShader->SetColorBufferID(_bufferBuilder->GetColorBufferID());
		}

		_shader = _cubeGeometryShader;
	}
	else if (_shaderType == QUAD_GEOMETRY_SHADER)
	{
		if (!_quadGeometryShader)
		{
			_quadGeometryShader = new QuadGeometryShader();
			_quadGeometryShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_quadGeometryShader->SetColorBufferID(_bufferBuilder->GetColorBufferID());
		}

		_shader = _quadGeometryShader;
	}
	else if (_shaderType == SPHERE_GEOMETRY_SHADER)
	{
		if (!_sphereGeometryShader)
		{
			_sphereGeometryShader = new SphereGeometryShader();
			_sphereGeometryShader->SetVertexBufferID(_bufferBuilder->GetVertexBufferID());
			_sphereGeometryShader->SetColorBufferID(_bufferBuilder->GetColorBufferID());
		}

		_shader = _sphereGeometryShader;
	}
	else if (_shaderType == SSAO_GEOMETRY_PASS_SHADER)
	{
		if (!_ssaoGeometryPassShader)
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
	else if (_shaderType == SSAO_MAP_OVERLAY_SHADER)
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

Shader* ShaderCreator::GetShader(int shaderType)
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
	else if (_shaderType == CUBE_GEOMETRY_SHADER)
	{
		return _cubeGeometryShader;
	}
	else if (_shaderType == QUAD_GEOMETRY_SHADER)
	{
		return _quadGeometryShader;
	}
	else if (_shaderType == SPHERE_GEOMETRY_SHADER)
	{
		return _sphereGeometryShader;
	}


	else if (_shaderType == PHONG_PER_VERTEX_SHADER)
	{
		return _phongPerVertexShader;
	}
	else if (_shaderType == PHONG_PER_PIXEL_SHADER)
	{
		return _phongPerPixelShader;
	}
	else if (_shaderType == PHONG_COLOR_SHADER)
	{
		return _phongColorShader;
	}
	else if (_shaderType == PHONG_WITH_TEXTURE_PER_VERTEX_SHADER)
	{
		return _phongWithTexPerVertexShader;
	}
	else if (_shaderType == PHONG_WITH_TEXTURE_PER_PIXEL_SHADER)
	{
		return _phongWithTexPerPixelShader;
	}


	else if (_shaderType == PBR_SHADER)
	{
		return _pbrShader;
	}
	else if (_shaderType == PBR_WITH_TEXTURE_SHADER)
	{
		return _pbrWithTextureShader;
	}
	else if (_shaderType == PBR_COLOR_SHADER)
	{
		return _pbrColorShader;
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

Shader* ShaderCreator::GetCurrentShader()
{
	return _shader;
}

unsigned int ShaderCreator::GetCurrentShaderType()
{
	return _shaderType;
}

ShaderCreator::~ShaderCreator()
{
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

	if (_cubeGeometryShader)
	{
		delete _cubeGeometryShader;
		_cubeGeometryShader = NULL;
	}

	if (_quadGeometryShader)
	{
		delete _quadGeometryShader;
		_quadGeometryShader = NULL;
	}

	if (_sphereGeometryShader)
	{
		delete _sphereGeometryShader;
		_sphereGeometryShader = NULL;
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

	if (_pbrColorShader)
	{
		delete _pbrColorShader;
		_pbrColorShader = NULL;
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

	if (_ssaoMapOverlayShader)
	{
		delete _ssaoMapOverlayShader;
		_ssaoMapOverlayShader = NULL;
	}

	if (_ssaoGeometryPassShader)
	{
		delete _ssaoGeometryPassShader;
		_ssaoGeometryPassShader = NULL;
	}
}
