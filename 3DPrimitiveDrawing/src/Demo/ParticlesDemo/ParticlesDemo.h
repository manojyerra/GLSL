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
	int _sw;
	int _sh;

	int _numLoaders;
	std::vector<ParticleRenderer*> _particleRendererVec;

	Floor* _floor;
	GLFBO* _fbo;
	GLTexture* _texture;
	bool _drawAllParticles;

public:
	ParticlesDemo(int sw, int sh);
	~ParticlesDemo();
	void SetScreenSize(int sw, int sh);
	void SetFloorVisible(bool visible);
	Floor* GetFloor();
	void Reset();
	void Draw();
};

#endif
