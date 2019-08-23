#ifndef PBRCubeGeometryShader_H
#define PBRCubeGeometryShader_H

#include "PBRShader.h"

class PBRCubeGeometryShader : public Shader
{
private:
	float _metallic;
	float _roughness;
	float _halfLen;
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