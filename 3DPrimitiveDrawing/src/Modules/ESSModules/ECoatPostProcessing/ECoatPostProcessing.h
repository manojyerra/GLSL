#ifndef ECoatPost_H
#define ECoatPost_H

#include "Floor.h"
#include "Module.h"
#include "GLMeshManager.h"
#include "GLTexture.h"
#include "TimeLineFrame.h"
#include "SUIActionListener.h"
#include "ParticleRenderer.h"
#include "ECoatResultReader.h"
#include "ECoatColorBar.h"
#include "ECoatAssetsBuilder.h"

class ECoatPostProcessing : public Module, public SUIActionListener
{
private:
	float _sw;
	float _sh;
	
	GLTexture* _texture;
	bool _needAllParticlesDraw;

	Floor* _floor;
	GLMeshRenderer* _carBody;
	GLMeshManager* _meshManager;
	ParticleRenderer* _particleRenderer;

	ECoatResultReader* _resultReader;
	ECoatAssetsBuilder* _assetsBuilder;
	ECoatColorBar* _colorBar;
	TimeLineFrame* _timeLineFrame;

	void ApplyContour(int frameNum);
	void SetGLStates();
	char* GetParticleColorBuf(int frameNum, unsigned int* bufSize);
	void DrawObjects(bool drawAllParticles);

public:
	ECoatPostProcessing(unsigned int sw, unsigned int sh, int argc, char** argv);
	~ECoatPostProcessing();

	void SetScreenSize(unsigned int sw, unsigned int sh) override;
	void OnModuleChange(unsigned int sw, unsigned int sh) override;
	void Update(float deltaTime) override;
	void Draw() override;
	void actionPerformed(SUIActionEvent e) override;
};

#endif
