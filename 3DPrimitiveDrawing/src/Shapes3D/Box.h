#ifndef Box_H
#define Box_H

#include "Macros.h"
#include "Shape.h"
#include "RandomColor.h"
#include "ShaderProgram.h"
#include "GLBatch.h"
#include "GLMat.h"
#include "glm/glm.hpp"
#include "GLMeshRenderer.h"

class Box : public Shape
{
private:
	float _w;
	float _h;
	float _d;

	GLMat _scaleMat;
	RandomColor _randomColor;

	GLMeshRenderer* _meshRenderer;

	void GenerateGeometry();

public:
	Box();
	Box(Box* box);
	Box(float* mat, glm::vec3 size);
	Box(float x, float y, float z, float w, float h, float d);
	~Box();

	void Set(Box* box);

	static Box CalcAABB(float* vertexBuf, int arrSize);
	static Box CalcBoundingBox(float* vertexBuf, int arrSize);
	static Box GetBoundingBoxAfterRotXYZ(float* vertexBuf, int arrSize, float xAng, float yAng, float zAng);

	void SetPos(float x, float y, float z);
	void SetSize(float w, float h, float d);
	glm::vec3 GetPos();
	glm::vec3 GetSize();
	float Volume();
	vector<glm::vec3> GetAABBVertices();

	void Draw();
};

#endif
