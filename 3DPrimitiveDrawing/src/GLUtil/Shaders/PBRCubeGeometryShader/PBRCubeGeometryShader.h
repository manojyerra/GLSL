#ifndef PBRCubeGeometryShader_H
#define PBRCubeGeometryShader_H

#include "PBRShader.h"

class PBRCubeGeometryShader : public Shader
{
private:
	float _metallic = 1.0f;
	float _roughness = 0.3f;
	float _halfLen = 0.00375f / 2.0f;
	std::map<std::string, LightInfo> _lightMap;

	ShaderProgram* _shaderProgram;

public:
	PBRCubeGeometryShader();
	~PBRCubeGeometryShader();
	
	void SetCubeHalfLen(float cubeHalfLen);
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif