#ifndef SSAOMapOverlayShader_H
#define SSAOMapOverlayShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

class SSAOMapOverlayShader : public Shader
{
private:
	float _occlusionLevel; // 0 to 1
	ShaderProgram* _shaderProgram;
	bool _use2DCam;

public:
	SSAOMapOverlayShader();
	~SSAOMapOverlayShader();

	void Set2DCamera(bool enable);
	void SetOcclusionLevel(float level);
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif