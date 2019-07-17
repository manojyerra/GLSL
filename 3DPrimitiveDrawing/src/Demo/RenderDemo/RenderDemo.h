#ifndef RenderDemo_H
#define RenderDemo_H

#include "GLMeshRenderer.h"
#include "Floor.h"
#include "ShaderFrame.h"

class RenderDemo
{
private:
	int _sw;
	int _sh;

	GLMeshRenderer* _model1;
	Floor* _floor;
	ShaderFrame* _shaderFrame;

public:
	RenderDemo(int sw, int sh);
	~RenderDemo();

	void Draw();
};

#endif
