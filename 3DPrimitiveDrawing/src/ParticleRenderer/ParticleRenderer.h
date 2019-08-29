#ifndef ParticleRenderer_H
#define ParticleRenderer_H

#include "GLMeshRenderer.h"
#include "BinaryObjReader.h"
#include "BufferInfo.h"
#include "glm/glm.hpp"
#include "AABB.h"


class ParticleRenderer
{
	GLMat _modelMat;
	GLMeshRenderer* _allParticlesRenderer;
	GLMeshRenderer* _fewParticlesRenderer;
	unsigned int _skipNumVertex;
	AABB _bBox;
	bool _hasNormals;
	float _particleLen;

	static const unsigned int BYTES_PER_VERTEX = 12;
	static const unsigned int BYTES_PER_COLOR = 3;

	BufferInfo GenerateLowPolyVertexBuf(BufferInfo* highPolyVerBufInfo);
	BufferInfo GenerateLowPolyColorBuf(BufferInfo* lowPolyVerBufInfo);

	GLMeshRenderer* CreateAllParticlesRenderer(BufferInfo* vertexBufInfo, BufferInfo* normalBufInfo, int shaderID);
	GLMeshRenderer* CreateFewParticlesRenderer(BufferInfo* highPolyVerBufInfo, BufferInfo* highPolyNormalBufInfo, int shaderID);

public:
	enum
	{
		CUBES,
		CUBES_WITH_LIGHTING
	};

	ParticleRenderer(std::string filePath);
	ParticleRenderer(BufferInfo* vertexBufInfo);
	ParticleRenderer(BufferInfo* vertexBufInfo, BufferInfo* normalBufInfo);

	~ParticleRenderer();

	bool SetDrawAs(int drawAs);
	void SetParticleLen(float particleLen);

	void SetPosition(glm::vec3& pos);
	void SetPosition(float x, float y, float z);
	void SetRotation(glm::vec3& rot);

	GLMat GetModelMat();
	glm::vec3 GetBBoxCenterAfterTransform();
	AABB GetBBox();
	glm::vec3 GetBBoxCenter();

	void UpdateColorBuffer(char* colorBuffer, unsigned int colorBufLen);

	void DrawForPicking();
	void DrawAllParticles();
	void DrawFewParticles();
};

#endif
