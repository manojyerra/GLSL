#ifndef ParticleRenderer_H
#define ParticleRenderer_H

#include "GLMeshRenderer.h"
#include "BinaryObjReader.h"

class ParticleRenderer
{
	GLMat _modelMat;
	GLMeshRenderer* _allParticlesRenderer;
	GLMeshRenderer* _fewParticlesRenderer;

	GLMeshRenderer* CreateAllParticlesRenderer(char* vertexBuf, unsigned int vertexBufLen);
	GLMeshRenderer* CreateAllParticlesRenderer(char* vertexBuf, unsigned int vertexBufLen, char* colorBuf, unsigned int colorBufLen);

	GLMeshRenderer* CreateFewParticlesRenderer(char* allParticleVertexBuf, unsigned int allParticleVertexBufLen);

public:
	ParticleRenderer(std::string filePath);
	ParticleRenderer(char* vertexBuf, unsigned int vertexBufLen);
	ParticleRenderer(char* vertexBuf, unsigned int vertexBufLen, char* colorBuf, unsigned int colorBufLen);


	~ParticleRenderer();

	void SetPosition(float x, float y, float z);

	void DrawAllParticles();
	void DrawFewParticles();
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
