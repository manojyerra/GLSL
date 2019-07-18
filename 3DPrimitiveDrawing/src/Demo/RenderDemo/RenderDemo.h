#ifndef RenderDemo_H
#define RenderDemo_H

#include "GLMeshRenderer.h"
#include "Floor.h"
#include "ShaderFrame.h"
#include "SUIActionListener.h"
#include "ModelDrawingFrame.h"
#include "ApplyShaderFrame.h"

class RenderDemo : public SUIActionListener 
{
private:
	int _sw;
	int _sh;

	int _numModels;

	GLMeshRenderer* _selectedModel;
	vector<GLMeshRenderer*> _modelVec;

	Floor* _floor;
	ShaderFrame* _shaderFrame;
	ModelDrawingFrame* _modelDrawingFrame;
	ApplyShaderFrame* _applyShaderFrame;

public:
	RenderDemo(int sw, int sh);
	~RenderDemo();

	void SetScreenSize(int sw, int sh);
	void Draw();
	void actionPerformed(SUIActionEvent e) override;

	void SetVisibleFrames(bool visible);
};

#endif
