#ifndef GLSSAO_H
#define GLSSAO_H

#include "GLSSAOBufferBuilder.h"
#include "GLMeshRenderer.h"
#include "ModelInfo.h"
#include "GLTexture.h"
#include "GLFBO.h"

class GLSSAO : public GLSSAOBufferBuilder
{
private:
	static const int NUM_SAMPLES = 128;

	unsigned int _noiseTexID;
	std::vector<glm::vec3> _ssaoSamples;

	GLMeshRenderer* _quadRenderer;
	GLMeshRenderer* _blurQuadRenderer;

	GLMat _modelMat;

	GLMeshRenderer* CreateQuadRenderer(unsigned int shaderType);
	void GenerateSampleKernelAndNoiseTexture();
	float lerp(float a, float b, float f);
	void DrawQuad();
	void DrawBlurQuad();

public:
	GLSSAO(float w, float h);
	~GLSSAO();

	void Begin();
	void End();

	unsigned int GetOcclusionMap();
};

#endif
