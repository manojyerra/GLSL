#ifndef Sphere_H
#define Sphere_H

#include "Shape.h"
#include "ShaderProgram.h"
#include "GLUtil/GLBuffer.h"

class Sphere : public Shape
{
private:
	float _r;

	GLuint _vertexBufferID;
	GLuint _colorBufferID;
	GLuint _vertexCount;
	ShaderProgram* _shaderProgram;

	void GenerateBufferID();
	void rot(int axis, float angleInDegrees, float x, float y, float z, float* newX, float* newY, float* newZ);

public:
	Sphere(float x, float y, float z, float r);
	Sphere(Sphere* sphere);
	Sphere(CVector3 pos, float r);
	Sphere(float* mat, float r);
	~Sphere();

	float GetRadius();
	void SetRadius(float r);
	static Sphere CalcBoundingSphere(float* vertexBuf, int arrSize);
	float Volume();

	void Draw();
};

#endif
