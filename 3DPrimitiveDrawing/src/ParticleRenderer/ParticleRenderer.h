#ifndef ParticleRenderer_H
#define ParticleRenderer_H

#include "GLMeshRenderer.h"
#include "BinaryObjReader.h"
#include "glm/glm.hpp"

class ParticleRenderer
{
	GLMat _modelMat;
	GLMeshRenderer* _allParticlesRenderer;
	GLMeshRenderer* _fewParticlesRenderer;
	unsigned int _skipNumVertex;
	glm::vec3 _bBoxCenter;

	static const unsigned int BYTES_PER_VERTEX = 12;
	static const unsigned int BYTES_PER_COLOR = 3;

	GLMeshRenderer* CreateAllParticlesRenderer(char* vertexBuf, unsigned int vertexBufLen);
	GLMeshRenderer* CreateFewParticlesRenderer(char* allParticleVertexBuf, unsigned int allParticleVertexBufLen);

public:
	ParticleRenderer(std::string filePath);
	ParticleRenderer(char* vertexBuf, unsigned int vertexBufLen);
	//ParticleRenderer(char* vertexBuf, unsigned int vertexBufLen, char* colorBuf, unsigned int colorBufLen);
	~ParticleRenderer();

	void SetPosition(float x, float y, float z);
	void UpdateColorBuffer(char* colorBuffer, unsigned int colorBufLen);
	glm::vec3 GetBBoxCenter();

	void DrawAllParticles();
	void DrawFewParticles();
};

#endif


//GLMeshRenderer* CreateAllParticlesRenderer(char* vertexBuf, unsigned int vertexBufLen, char* colorBuf, unsigned int colorBufLen);

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
