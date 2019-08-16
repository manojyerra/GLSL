#ifndef ShaderCreator_H
#define ShaderCreator_H

#include "BasicShader.h"
#include "ColorShader.h"
#include "UVShader.h"
#include "PhongShader.h"
#include "PhongWithTextureShader.h"
#include "PhongColorShader.h"
#include "PBRShader.h"
#include "PBRWithTextureShader.h"
#include "PBRColorShader.h"
#include "CubeGeometryShader.h"
#include "QuadGeometryShader.h"
#include "SphereGeometryShader.h"
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
	PHONG_COLOR_SHADER,
	PBR_SHADER,
	PBR_WITH_TEXTURE_SHADER,
	PBR_COLOR_SHADER,
	CUBE_GEOMETRY_SHADER,
	QUAD_GEOMETRY_SHADER,
	SPHERE_GEOMETRY_SHADER,
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
	CubeGeometryShader* _cubeGeometryShader;
	QuadGeometryShader* _quadGeometryShader;
	SphereGeometryShader* _sphereGeometryShader;
	PhongShader* _phongPerVertexShader;
	PhongShader* _phongPerPixelShader;
	PhongColorShader* _phongColorShader;
	PhongWithTextureShader* _phongWithTexPerVertexShader;
	PhongWithTextureShader* _phongWithTexPerPixelShader;
	PBRShader* _pbrShader;
	PBRColorShader* _pbrColorShader;
	PBRWithTextureShader* _pbrWithTextureShader;
	SSAOShader* _ssaoShader;
	SSAOBlurShader* _ssaoBlurShader;
	SSAOMapOverlayShader* _ssaoMapOverlayShader;
	SSAOGeometryPassShader* _ssaoGeometryPassShader;

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