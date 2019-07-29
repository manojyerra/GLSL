#ifdef _ENABLE_DEMO

#ifndef RenderDemo_H
#define RenderDemo_H

#include "GLSSAOBufferBuilder.h"
#include "GLMeshRenderer.h"
#include "Floor.h"

#include "ShaderFrame.h"
#include "SUIActionListener.h"
#include "ModelVisibilityFrame.h"
#include "ModelSelectionFrame.h"
#include "GLTexture.h"
#include "GLSSAO.h"

class RenderDemo : public SUIActionListener
{
private:
	float _sw;
	float _sh;
	int _numModels;

	GLMeshRenderer* _selectedModel;
	vector<GLMeshRenderer*> _modelVec;
	Floor* _floor;

	bool _enableSSAO;
	GLSSAO* _ssao;
	GLTexture* _texture;

	ShaderFrame* _shaderFrame;
	ModelVisibilityFrame* _modelVisibilityFrame;
	ModelSelectionFrame* _modelSelectionFrame;

	void DrawObjectsForSSAO();

public:
	RenderDemo(float sw, float sh);
	~RenderDemo();

	void SetScreenSize(float sw, float sh);
	void Draw();
	void actionPerformed(SUIActionEvent e) override;

	void SetVisibleFrames(bool visible);
	void SetFloorVisible(bool visible);
	Floor* GetFloor();

	void SetEnableSSAO(bool enable);
};

#endif

#endif