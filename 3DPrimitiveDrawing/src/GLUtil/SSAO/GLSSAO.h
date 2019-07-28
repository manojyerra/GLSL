#ifndef GLSSAO_H
#define GLSSAO_H

#include "GLSSAOBufferBuilder.h"
#include "GLMeshRenderer.h"
#include "ModelInfo.h"

class GLSSAO : public GLSSAOBufferBuilder
{
private:
	unsigned int _noiseTexID;
	std::vector<glm::vec3> _ssaoSamples;

	GLMeshRenderer* _quadRenderer;
	GLMat _modelMat;

	GLMeshRenderer* CreateQuadRenderer();
	void GenerateSampleKernelAndNoiseTexture();
	float lerp(float a, float b, float f);

public:
	GLSSAO(float w, float h);
	~GLSSAO();

	void DrawQuad();
};

#endif
