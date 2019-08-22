#ifndef ParticleRenderer_H
#define ParticleRenderer_H

#include "GLMeshRenderer.h"
#include "BinaryObjReader.h"
#include "BufferInfo.h"
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

	BufferInfo GenerateLowPolyVertexBuf(BufferInfo* highPolyVerBufInfo);
	BufferInfo GenerateLowPolyColorBuf(BufferInfo* lowPolyVerBufInfo);

	GLMeshRenderer* CreateAllParticlesRenderer(BufferInfo* vertexBufInfo, BufferInfo* normalBufInfo, int shaderID);
	GLMeshRenderer* CreateFewParticlesRenderer(BufferInfo* highPolyVerBufInfo, BufferInfo* highPolyNormalBufInfo, int shaderID);

public:
	enum
	{
		DRAW_AS_CUBES,
		DRAW_AS_QUADS,
		DRAW_AS_SPHERES,
		DRAW_AS_POINTS
	};

	ParticleRenderer(std::string filePath);
	ParticleRenderer(BufferInfo* vertexBufInfo);
	ParticleRenderer(BufferInfo* vertexBufInfo, BufferInfo* normalBufInfo);

	~ParticleRenderer();

	void SetDrawAs(int drawAs);

	void SetPosition(float x, float y, float z);
	void UpdateColorBuffer(char* colorBuffer, unsigned int colorBufLen);
	glm::vec3 GetBBoxCenter();

	void DrawAllParticles();
	void DrawFewParticles();
};

#endif
