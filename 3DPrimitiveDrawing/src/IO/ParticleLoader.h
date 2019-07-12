#ifndef ParticleLoader_H
#define ParticleLoader_H

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

class ParticleLoader
{
	ShaderProgram* _shaderProgram;
	GLuint _vertexBufferID;
	GLuint _colorBufferID;
	GLuint _vertexCount;

	GLMat _modelMat;
	float _cubeHalfLen;
	int _methodNum;

public:
	ParticleLoader();
	~ParticleLoader();

	void Draw();
};

#endif