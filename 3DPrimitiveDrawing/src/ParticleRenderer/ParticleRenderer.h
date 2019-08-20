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

	GLMeshRenderer* CreateAllParticlesRenderer(char* vertexBuf, unsigned int vertexBufLen, 
												char* normalBuf, unsigned int normalBufSize);
	GLMeshRenderer* CreateFewParticlesRenderer(char* allParticleVertexBuf, unsigned int allParticleVertexBufLen, 
												char* normalBuf, unsigned int normalBufSize);

public:
	enum
	{
		DRAW_AS_CUBES,
		DRAW_AS_QUADS,
		DRAW_AS_SPHERES,
		DRAW_AS_POINTS
	};

	ParticleRenderer(std::string filePath);
	ParticleRenderer(char* vertexBuf, unsigned int vertexBufSize);
	ParticleRenderer(char* vertexBuf, unsigned int vertexBufSize, char* normalBuf, unsigned int normalBufSize);

	~ParticleRenderer();

	void SetDrawAs(int drawAs);

	void SetPosition(float x, float y, float z);
	void UpdateColorBuffer(char* colorBuffer, unsigned int colorBufLen);
	glm::vec3 GetBBoxCenter();

	void DrawAllParticles();
	void DrawFewParticles();
};

#endif
