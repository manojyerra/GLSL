#ifndef ECoatPost_H
#define ECoatPost_H

#include "Floor.h"
#include "Module.h"
#include "GLMeshManager.h"
#include "GLTexture.h"
#include "ECoatUI.h"
#include "SUIActionListener.h"
#include "ECoatParticleDataMgr.h"
#include "ECoatResultReader.h"
#include "ColorBar.h"
#include "ECoatAssetsBuilder.h"

class ECoatPostProcessing : public Module, public SUIActionListener
{
private:
	float _sw;
	float _sh;
	bool _needAllParticlesDraw;
	bool _drawAnodes;

	GLTexture* _texture;

	Floor* _floor;
	GLMeshRenderer* _carBody;
	GLMeshManager* _meshManager;
	ECoatParticleDataMgr* _particleMgr;

	ColorBar* _colorBar;
	ECoatUI* _timeLineFrame;

	ECoatAssetsBuilder* _assetsBuilder;
	ECoatAssetsBuilder* _resultFileReader;

	bool _showThickness;
	double _selectedThickness;
	glm::ivec2 _posForThickness;

	void SetGLStates();
	void DrawObjects(bool drawAllParticles);
	void UpdateSolidPivot();

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
