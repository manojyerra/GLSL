#ifndef GLSSAOTexture_H
#define GLSSAOTexture_H

#include "GLMeshRenderer.h"

class GLSSAOTexture
{
private:
	GLMeshRenderer* _meshRenderer;
	GLMat _modelMat;

	void GenerateGeometry();

public:
	GLSSAOTexture(float x, float y, float w, float h);
	~GLSSAOTexture();

	void SetBounds(float x, float y, float w, float h);
	void SetGPositionTexID(unsigned int texID);
	void SetGNormalTexID(unsigned int texID);
	void SetNoiseTexID(unsigned int texID);
	void SetSamples(std::vector<glm::vec3> samples);

	void Draw();
};

#endif
