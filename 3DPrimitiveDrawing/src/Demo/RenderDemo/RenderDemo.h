#ifndef RenderDemo_H
#define RenderDemo_H

#include "GLMeshRenderer.h"
#include "Floor.h"

class RenderDemo
{
private:
	GLMeshRenderer* _model1;
	Floor* _floor;
	int _sw;
	int _sh;

public:
	RenderDemo(int sw, int sh);
	~RenderDemo();

	void Draw();
};

#endif
