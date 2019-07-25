#ifndef RenderDemo_H
#define RenderDemo_H

#include "GLSSAO.h"
#include "GLMeshRenderer.h"
#include "Floor.h"

#include "ShaderFrame.h"
#include "SUIActionListener.h"
#include "ModelVisibilityFrame.h"
#include "ModelSelectionFrame.h"
#include "GLTexture.h"
#include "GLSSAOTexture.h"

class RenderDemo : public SUIActionListener
{
private:
	float _sw;
	float _sh;
	int _numModels;

	GLSSAO * _ssao;
	GLTexture* _texture;
	GLSSAOTexture* _ssaoTexture;

	GLMeshRenderer* _selectedModel;
	vector<GLMeshRenderer*> _modelVec;
	Floor* _floor;

	ShaderFrame* _shaderFrame;
	ModelVisibilityFrame* _modelVisibilityFrame;
	ModelSelectionFrame* _modelSelectionFrame;

public:
	RenderDemo(float sw, float sh);
	~RenderDemo();

	void SetScreenSize(float sw, float sh);
	void Draw();
	void actionPerformed(SUIActionEvent e) override;

	void SetVisibleFrames(bool visible);
	void SetFloorVisible(bool visible);
	Floor* GetFloor();
};

#endif
