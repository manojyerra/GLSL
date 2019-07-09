#ifndef ParticleLoader_H
#define ParticleLoader_H

#include "Vector3.h"
#include "ShaderProgram.h"
#include "GLMat.h"


class Tuple
{
public:
	float x;
	float y;
	float z;

	Tuple(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	bool operator ==(const Tuple& obj) const
	{
		return ((int)(x * 1000) == (int)(obj.x * 1000) &&
			(int)(y * 1000) == (int)(obj.y * 1000) &&
			(int)(z * 1000) == (int)(obj.z * 1000));
	}
};


class MyHashFunction {
public:
	size_t operator()(const Tuple& t) const
	{
		return (int)(t.x * 1000) + (int)(t.y * 1000) + (int)(t.z * 1000);
	}
};


class ParticleLoader
{
	ShaderProgram* _shaderProgram;
	GLuint _vertexBufferID;
	GLuint _colorBufferID;
	GLuint _vertexCount;

	GLMat _modelMat;

public:
	ParticleLoader();
	~ParticleLoader();

	void Draw();
};

#endif