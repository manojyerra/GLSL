#ifndef ECoatPost_H
#define ECoatPost_H

#include "Floor.h"
#include "Module.h"
#include "GLMeshManager.h"
#include "TimeLineFrame.h"
#include "SUIActionListener.h"
#include "ParticleRenderer.h"
#include "ECoatResultReader.h"
#include "ECoatColorBar.h"
#include "ECoatAssetsBuilder.h"

class ECoatPost : public Module, public SUIActionListener
{
private:
	float _sw;
	float _sh;

	Floor* _floor;
	GLMeshManager* _meshManager;
	ParticleRenderer* _particleRenderer;
	ECoatAssetsBuilder* _assetsBuilder;

	ECoatColorBar* _colorBar;
	TimeLineFrame* _timeLineFrame;
	ECoatResultReader* _resultReader;

	void ApplyContour(int frameNum);

public:
	ECoatPost(unsigned int sw, unsigned int sh, int argc, char** argv);
	~ECoatPost();

	void SetScreenSize(unsigned int sw, unsigned int sh) override;
	void OnModuleChange(unsigned int sw, unsigned int sh) override;
	void Update(float deltaTime) override;
	void Draw() override;
	void actionPerformed(SUIActionEvent e) override;
};

#endif
