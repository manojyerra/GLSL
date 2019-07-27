#ifndef GLSSAO_H
#define GLSSAO_H

#include "GLSSAOBufferBuilder.h"
#include "GLMeshRenderer.h"
#include "ModelInfo.h"

class GLSSAO : public GLSSAOBufferBuilder
{
private:
	GLMeshRenderer* _quadRenderer;
	GLMat _modelMat;

	ModelInfo CreateModeInfo();

public:
	GLSSAO(float w, float h);
	~GLSSAO();

	void Draw();
};

#endif


//void SetBounds(float x, float y, float w, float h);