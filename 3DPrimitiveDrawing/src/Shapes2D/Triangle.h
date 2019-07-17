#ifndef Triangle_H
#define Triangle_H

#include "GLMat.h"
#include "glm/glm.hpp"
#include "GLMeshRenderer.h"

class Triangle
{
	GLMat _modelMat;
	GLMeshRenderer* _meshRenderer;

public:
	Triangle(glm::vec3& v1, glm::vec3& v2, glm::vec3& v3);
	~Triangle();

	void Draw();
};

#endif