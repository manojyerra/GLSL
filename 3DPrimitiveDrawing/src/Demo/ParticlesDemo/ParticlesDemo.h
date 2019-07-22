#ifndef ParticlesDemo_H
#define ParticlesDemo_H

#include "Cam.h"
#include "Cam2D.h"
#include "Floor.h"
#include "Input.h"
#include "GLFBO.h"
#include "GLTexture.h"
#include "ParticleLoader.h"
#include <vector>

class ParticlesDemo
{
private:
	int _sw;
	int _sh;

	int _numLoaders;
	std::vector<ParticleLoader*> _particleLoaderVec;

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
