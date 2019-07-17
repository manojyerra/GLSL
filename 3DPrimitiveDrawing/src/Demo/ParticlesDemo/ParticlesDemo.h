#ifndef ParticlesDemo_H
#define ParticlesDemo_H

#include "Cam.h"
#include "Cam2D.h"
#include "Floor.h"
#include "Input.h"
#include "GLFBO.h"
#include "GLTexture.h"
#include "ParticleLoader.h"

class ParticlesDemo
{
private:
	int _sw;
	int _sh;

	ParticleLoader* _particleLoader1;
	ParticleLoader* _particleLoader2;
	ParticleLoader* _particleLoader3;
	ParticleLoader* _particleLoader4;
	ParticleLoader* _particleLoader5;

	GLFBO* _fbo;
	GLTexture* _texture;
	bool _drawAllParticles;

public:
	ParticlesDemo(int sw, int sh);
	~ParticlesDemo();

	void Draw(Floor* floor);
};

#endif
