#ifndef ShaderCreator_H
#define ShaderCreator_H

#include "BasicShader.h"
#include "ColorShader.h"
#include "UVShader.h"
#include "PhongShader.h"
#include "PhongWithTextureShader.h"
#include "PBRShader.h"
#include "PBRWithTextureShader.h"
#include "PBRColorShader.h"
#include "CubeGeometryShader.h"
#include "SSAOGeometryPassShader.h"
#include "SSAOShader.h"
#include "SSAOBlurShader.h"
#include "SSAOMapOverlayShader.h"
#include "GLBufferBuilder.h"

enum {
	BASIC_SHADER,
	COLOR_SHADER,
	UV_SHADER,
	PHONG_PER_VERTEX_SHADER,
	PHONG_PER_PIXEL_SHADER,
	PHONG_WITH_TEXTURE_PER_VERTEX_SHADER,
	PHONG_WITH_TEXTURE_PER_PIXEL_SHADER,
	PBR_SHADER,
	PBR_WITH_TEXTURE_SHADER,
	PBR_COLOR_SHADER,
	CUBE_GEOMETRY_SHADER,
	SSAO_GEOMETRY_PASS_SHADER,
	SSAO_SHADER,
	SSAO_BLUR_SHADER,
	SSAO_MAP_OVERLAY_SHADER
};

class ShaderCreator
{
private:
	BasicShader* _basicShader;
	ColorShader* _colorShader;
	UVShader* _uvShader;
	PhongShader* _phongPerVertexShader;
	PhongShader* _phongPerPixelShader;
	PhongWithTextureShader* _phongWithTexPerVertexShader;
	PhongWithTextureShader* _phongWithTexPerPixelShader;
	PBRShader* _pbrShader;
	PBRWithTextureShader* _pbrWithTextureShader;
	PBRColorShader* _pbrColorShader;
	CubeGeometryShader* _cubeGeometryShader;
	SSAOGeometryPassShader* _ssaoGeometryPassShader;
	SSAOShader* _ssaoShader;
	SSAOBlurShader* _ssaoBlurShader;
	SSAOMapOverlayShader* _ssaoMapOverlayShader;

	Shader* _shader;
	int _shaderType;
	
	GLBufferBuilder* _bufferBuilder;

public:
	ShaderCreator(unsigned int shaderType, GLBufferBuilder* bufferBuilder);
	~ShaderCreator();

	void SetShader(int shaderType);
	Shader* GetShader(int shaderType);
	Shader* GetCurrentShader();
	unsigned int GetCurrentShaderType();
};

#endif