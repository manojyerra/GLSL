#ifndef Rect2D_H
#define Rect2D_H

#include "Math/Vector3.h"
#include "ShaderProgram.h"

class Rect2D
{
	ShaderProgram* _shaderProgram;
	
	GLint _vertexBufferID;
	GLuint _colorBufferID;
	GLuint _vertexCount;

	float _x;
	float _y;
	float _w;
	float _h;

public:
	Rect2D(float x, float y, float w, float h);
	void Draw();
	~Rect2D();
};

#endif