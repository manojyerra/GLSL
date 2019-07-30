#ifndef SSAOMapOverlayShader_H
#define SSAOMapOverlayShader_H

#include "ShaderProgram.h"
#include "GLMat.h"
#include "Shader.h"

class SSAOMapOverlayShader : public Shader
{
private:
	unsigned int _vertexBufferID;
	unsigned int _uvBufferID;
	unsigned int _textureID;
	float _alpha;
	float _occlusionLevel; // 0 to 1

	ShaderProgram* _shaderProgram;

	GLMat _modelMat;
	bool _use2DCam;

public:
	SSAOMapOverlayShader();
	~SSAOMapOverlayShader();

	void SetVertexBufferID(unsigned int bufferID);
	void SetUVBufferID(unsigned int bufferID);
	void SetTextureID(unsigned int textureID);
	void Set2DCamera(bool enable);

	void SetOcclusionLevel(float level);
	void SetModelMatrix(float* mat) override;
	void SetAlpha(float alpha) override;
	void Begin() override;
	void SetUniformsAndAttributes() override;
	void End() override;
};

#endif