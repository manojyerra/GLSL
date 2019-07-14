#ifndef Box_H
#define Box_H

#include "Macros.h"
#include "Shape.h"
#include "RandomColor.h"
#include "ShaderProgram.h"
#include "GLBuffer.h"
#include "GLMat.h"
#include "glm/glm.hpp"

class Box : public Shape
{
private:
	float _w;
	float _h;
	float _d;

	GLMat _scaleMat;

	RandomColor _randomColor;

	GLuint _vertexBufferID;
	GLuint _colorBufferID;
	GLuint _vertexCount;
	ShaderProgram* _shaderProgram;

	GLBuffer* _buffer;
	
	void GenerateBufferID();
	void InitCommon();

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
