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
#include "ColorBar.h"
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

	TimeLineFrame* _timeLineFrame;
	ColorBar* _colorBar;

	ECoatResultReader* _resultReader;
	ECoatAssetsBuilder* _assetsBuilder;

	void SetGLStates();
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
