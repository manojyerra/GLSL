#ifndef Cone_H
#define Cone_H

#include "Shape.h"
#include "RandomColor.h"
#include "ShaderProgram.h"
#include "GLBuffer.h"
#include "GLMat.h"
#include "glm/glm.hpp"
#include "GLMeshRenderer.h"

class Cone : public Shape
{
private:
	float _r;
	float _h;
	GLMat _scaleMat;

	RandomColor _randomColor;

	GLMeshRenderer* _meshRenderer;

	void GenerateGeometry();

public:
	Cone();
	Cone(Cone* cone);
	Cone(float* mat, float r, float h);
	Cone(float x, float y, float z, float r, float h);
	~Cone();

	glm::vec3 GetPos();

	float GetRadius();
	float GetHeight();

	void SetRadius(float r);
	void SetHeight(float h);

	float Volume();

	static float CalcRadius(float* vertexBuf, int arrSize, float height);
	static Cone CalcBoundingCone(float* vertexBuf, int arrSize);
	//static Cone GetBoundingConeAfterRotXYZ(float* vertexBuf, int arrSize, float xAng, float yAng, float zAng);

	void Draw();
};

#endif
