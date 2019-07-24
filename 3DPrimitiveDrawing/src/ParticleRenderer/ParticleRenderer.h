#ifndef ParticleRenderer_H
#define ParticleRenderer_H

#include "GLMeshRenderer.h"

class ParticleRenderer
{
	GLMat _modelMat;
	GLMeshRenderer* _normalPolyRenderer;
	GLMeshRenderer* _lowPolyRenderer;

public:
	ParticleRenderer();
	~ParticleRenderer();

	void SetPosition(float x, float y, float z);

	void DrawAllParticles();
	void DrawLowPolyParticles();
};

#endif


//class Tuple
//{
//public:
//	float x;
//	float y;
//	float z;
//
//	Tuple(float x, float y, float z)
//	{
//		this->x = x;
//		this->y = y;
//		this->z = z;
//	}
//
//	bool operator ==(const Tuple& obj) const
//	{
//		return ((int)(x * 1000) == (int)(obj.x * 1000) &&
//			(int)(y * 1000) == (int)(obj.y * 1000) &&
//			(int)(z * 1000) == (int)(obj.z * 1000));
//	}
//};