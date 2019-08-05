#ifdef _ENABLE_DEMO

#ifndef ParticlesDemo_H
#define ParticlesDemo_H

#include "Floor.h"
#include "GLFBO.h"
#include "GLTexture.h"
#include "ParticleRenderer.h"
#include <vector>

class ParticlesDemo
{
private:
	float _sw;
	float _sh;

	int _numLoaders;
	std::vector<ParticleRenderer*> _particleRendererVec;

	Floor* _floor;
	GLFBO* _fbo;
	GLTexture* _texture;
	bool _drawAllParticles;

public:
	ParticlesDemo(float sw, float sh);
	~ParticlesDemo();
	void SetScreenSize(float sw, float sh);
	void SetFloorVisible(bool visible);
	Floor* GetFloor();
	void Reset();
	void Draw();
};

#endif

#endif