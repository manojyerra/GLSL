#ifndef Cylinder_H
#define Cylinder_H

#include "Shape.h"
#include "RandomColor.h"
#include "GLBatch.h"
#include "GLMat.h"
#include "glm/glm.hpp"
#include "GLMeshRenderer.h"

class Cylinder : public Shape
{
private:
	float _r;
	float _h;
	GLMat _scaleMat;

	RandomColor _randomColor;

	GLMeshRenderer* _meshRenderer;

	void GenerateGeometry();

public:
	Cylinder();
	Cylinder(Cylinder* cylinder);
	Cylinder(float* mat, float r, float h);
	Cylinder(float x, float y, float z, float r, float h);
	~Cylinder();

	glm::vec3 GetPos();

	float GetRadius();
	float GetHeight();

	void SetRadius(float r);
	void SetHeight(float h);

	float Volume();

	static Cylinder CalcBoundingCylinder(float* vertexBuf, int arrSize);
	static Cylinder GetBoundingCylinderAfterRotXYZ(float* vertexBuf, int arrSize, float xAng, float yAng, float zAng);

	void Draw();
};

#endif
